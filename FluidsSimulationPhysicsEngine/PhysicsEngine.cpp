#include "PhysicsEngine.h"
#include <windows.h>

void* engineUpdateLoopThreadFunction(void * engineArg, int threadIndex)
{
	((PhysicsEngine*)engineArg)->engineUpdateLoop(threadIndex);
	return 0;
}

PhysicsEngine::~PhysicsEngine() {
	this->shouldStopEngine = true;
	for (int i = 0; i < this->totalBarriers; i++)
	{
		synchronizationBarriers[i]->Break();
	}

	for (int i = 0; i < this->runningThreads; i++)
	{
		if (this->engineUpdateLoopThreads[i]->joinable())
		{
			this->engineUpdateLoopThreads[i]->join();
		}
		delete this->engineUpdateLoopThreads[i];
	}

	delete this->engineUpdateLoopThreads;

	for (int i = 0; i < this->totalBarriers; i++)
	{
		delete synchronizationBarriers[i];
	}
	delete synchronizationBarriers;
}

Grid & PhysicsEngine::getUnsafeBodiesGrid()
{
	return this->bodiesGrid;
}

PhysicsEngine::PhysicsEngine() : bodiesGrid(QSize(400, 300), 2.5)
{
	this->lastMomentProcessingStarted = chrono::high_resolution_clock::now();
	this->shouldStopEngine = false;
	this->shouldRunLoop = false;
	this->runningThreads = max(1, thread::hardware_concurrency());
	this->totalBarriers = this->constantBarriersCount + this->calculationOperationsCount;
	this->synchronizationBarriers = new ThreadsBarrier*[this->totalBarriers];
	for (int i = 0; i < this->totalBarriers; i++)
	{
		synchronizationBarriers[i] = new ThreadsBarrier(runningThreads);
	}

	lockGridAddition = false;

	this->engineUpdateLoopThreads = new thread*[runningThreads];
	for (int i = 0; i < runningThreads; i++)
	{
		engineUpdateLoopThreads[i] = new thread(engineUpdateLoopThreadFunction, this, i);
	}
}

PhysicsEngine * PhysicsEngine::shared()
{
	return PhysicsEngine::sharedEngine;
}

void PhysicsEngine::resumeEngine() {
	this->shouldRunLoop = true;
	std::lock_guard<std::mutex> lock(shouldBeProcessingNextUpdateLoopLocker);
	shouldBeProcessingNextUpdateLoopConditional.notify_all();
}

void PhysicsEngine::pauseEngine() {
	this->shouldRunLoop = false;
	std::lock_guard<std::mutex> lock(shouldBeProcessingNextUpdateLoopLocker);
	shouldBeProcessingNextUpdateLoopConditional.notify_all();
}

void PhysicsEngine::addBodiesToGrid(BodiesVector bodies)
{
	if (lockGridAddition) {
		std::lock_guard<std::mutex> lock(bodiesToAddToGridLock);
		this->bodiesToAddToGrid.append(bodies);
	}
	else {
		std::lock_guard<std::mutex> lock(bodiesAccessLock);
		this->bodiesGrid.addBodiesToGrid(bodies);
		this->bodiesCount = this->bodiesGrid.bodiesCount();
	}
}

void PhysicsEngine::engineUpdateLoop(int threadIndex) {
	while (!shouldStopEngine)
	{
		{
			// wait until we are allowed to process the next update or the engine stops
			std::unique_lock<std::mutex> lock(shouldBeProcessingNextUpdateLoopLocker);
			shouldBeProcessingNextUpdateLoopConditional.wait(lock, [&] {return this->shouldRunLoop || shouldStopEngine; });
		}

		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			break;
		}

		// wait for every thread to come here, if pausing happens, then 
		// one or more threads will get stuck before reaching this point
		// enforcing the pause if one of the threads hasn't started yet
		this->synchronizationBarriers[0]->Await();

		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			break;
		}

		if (threadIndex == 0) {
			this->lockGridAddition = true;
			{
				std::lock_guard<std::mutex> lock(bodiesAccessLock);
				performAddCurrentBodiesToGrid();
				this->totalBodiesForProcessingLoop = this->bodiesGrid.bodiesCount();
			}
			this->lastMomentProcessingStarted = chrono::high_resolution_clock::now();
		}

		// wait for the main thread to finish locking the grid and calculating the 
		// start time.
		this->synchronizationBarriers[1]->Await();

		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			break;
		}

		for (int calculationOperation = 0; calculationOperation < this->calculationOperationsCount; calculationOperation++)
		{
			this->runUpdateBatch(threadIndex, calculationOperation);

			if (threadIndex == 0 && calculationOperation == this->calculationOperationsCount - 1) {
				bodiesAccessLock.lock();
			}
			// wait for all threads to finish calculating their values
			this->synchronizationBarriers[constantBarriersBeforeCalculationsCount + calculationOperation]->Await();

			// stop the loop when exiting the thread is required
			if (shouldStopEngine) {
				if (threadIndex == 0 && calculationOperation == this->calculationOperationsCount - 1) {
					bodiesAccessLock.unlock();
				}
				break;
			}
		}

		if (shouldStopEngine) {
			if (threadIndex == 0) {
				bodiesAccessLock.unlock();
			}
			break;
		}
		this->applyUpdates(threadIndex);

		// wait for all threads to finish applying updates
		this->synchronizationBarriers[calculationOperationsCount + constantBarriersBeforeCalculationsCount]->Await();


		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			if (threadIndex == 0) {
				bodiesAccessLock.unlock();
			}
			break;
		}

		if (threadIndex == 0) {
			performAddCurrentBodiesToGrid();
			this->bodiesGrid.updateBodiesInGrid();
			this->bodiesCount = this->bodiesGrid.bodiesCount();
			bodiesAccessLock.unlock();
			this->lockGridAddition = false;
			// main thread stalls others by not reaching it's awake
			// until the time since the last processing start is greater
			// than or equal to the engine's time delta
			auto timeSinceLastLoop = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - this->lastMomentProcessingStarted).count();
			auto timeBetweenLoops = int(this->timeDelta * 1000 * speedSlownessScale);
			this->fps = 1000.0 / max(timeSinceLastLoop, 1);
			if (timeSinceLastLoop < (timeBetweenLoops)) {
				Sleep(timeBetweenLoops - timeSinceLastLoop);
			}
		}
	}
}

void PhysicsEngine::performAddCurrentBodiesToGrid()
{
	std::lock_guard<std::mutex> lock(bodiesToAddToGridLock);
	if (this->bodiesToAddToGrid.length() == 0) {
		return;
	}
	this->bodiesGrid.addBodiesToGrid(this->bodiesToAddToGrid);
	this->bodiesToAddToGrid.clear();
}

void PhysicsEngine::runUpdateBatch(int threadIndex, int calculationOperation) {

	this->runFunctionOverThreadBodies(threadIndex, [&](Body* body, int index) {
		auto surroundingBodies = this->bodiesGrid.getBodySourroundingBodiesVectors(index);
		body->calculateInteractionWithBodies(surroundingBodies, calculationOperation);
	});
}

void PhysicsEngine::applyUpdates(int threadIndex) {
	this->runFunctionOverThreadBodies(threadIndex, [&](Body* body, int index) {
		body->applyInteraction();
	});
}

int PhysicsEngine::getBodiesCount() {
	return this->bodiesCount;
}

void PhysicsEngine::runFunctionOverBodies(const function <void(Body*)>&& func) {
	std::lock_guard<std::mutex> lock(bodiesAccessLock);
	auto bodies = this->bodiesGrid.getAllBodies();
	for (int i = 0; i < this->bodiesGrid.bodiesCount(); i++)
	{
		func(this->bodiesGrid.getBodyAtIndex(i));
	}
}

void PhysicsEngine::runFunctionOverThreadBodies(int threadIndex, const function <void(Body*, int)>&& func) {
	const int bodiesPerThread = this->totalBodiesForProcessingLoop / this->runningThreads;

	int bodiesForCurrentThreadStartIndex = bodiesPerThread * threadIndex;

	int bodiesForCurrentThreadEndIndex = (bodiesPerThread * (threadIndex + 1)) - 1;

	if (threadIndex == this->runningThreads - 1) {
		bodiesForCurrentThreadEndIndex += this->totalBodiesForProcessingLoop % this->runningThreads;
	}

	for (int i = bodiesForCurrentThreadStartIndex; i <= bodiesForCurrentThreadEndIndex; i++)
	{
		Body* body = this->bodiesGrid.getBodyAtIndex(i);
		func(body, i);
	}
}

PhysicsEngine* PhysicsEngine::sharedEngine = new PhysicsEngine();
