#include "PhysicsEngine.h"
#include <windows.h>

void* engineUpdateLoopThreadFunction(void * engineArg, int threadIndex)
{
	((PhysicsEngine*)engineArg)->engineUpdateLoop(threadIndex);
	return 0;
}

PhysicsEngine::~PhysicsEngine() {
	this->shouldStopEngine = true;
	for (int i = 0; i < this->barriersCount; i++)
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

	for (int i = 0; i < this->barriersCount; i++)
	{
		delete synchronizationBarriers[i];
	}
	delete synchronizationBarriers;
}

PhysicsEngine::PhysicsEngine() : bodiesGrid(QSize(24, 18), 3)
{
	this->lastMomentProcessingStarted = chrono::high_resolution_clock::now();
	this->shouldStopEngine = false;
	this->shouldRunLoop = false;
	this->runningThreads = max(1, thread::hardware_concurrency());
	this->synchronizationBarriers = new ThreadsBarrier*[this->barriersCount];
	for (int i = 0; i < this->barriersCount; i++)
	{
		synchronizationBarriers[i] = new ThreadsBarrier(runningThreads);
	}

	lockGrid = false;

	this->engineUpdateLoopThreads = new thread*[runningThreads];
	for (int i = 0; i < runningThreads; i++)
	{
		engineUpdateLoopThreads[i] = new thread(engineUpdateLoopThreadFunction, this, i);
	}
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
	std::lock_guard<std::mutex> lock(bodiesAccessLock);
	if (lockGrid) {
		this->bodiesToAddToGrid.append(bodies);
	}
	else {
		this->bodiesGrid.addBodiesToGrid(bodies);
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
			performAddCurrentBodiesToGrid();
			this->lockGrid = true;
			this->totalBodiesForProcessingLoop = this->bodiesGrid.bodiesCount();
			this->lastMomentProcessingStarted = chrono::high_resolution_clock::now();
		}

		// wait for the main thread to finish locking the grid and calculating the 
		// start time.
		this->synchronizationBarriers[1]->Await();

		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			break;
		}

		this->runUpdateBatch(threadIndex);

		// wait for all threads to finish calculating their values
		this->synchronizationBarriers[2]->Await();

		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			break;
		}

		this->applyUpdates(threadIndex);

		// wait for all threads to finish applying updates
		this->synchronizationBarriers[3]->Await();

		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			break;
		}

		if (threadIndex == 0) {
			performAddCurrentBodiesToGrid();
			this->lockGrid = false;
			// main thread stalls others by not reaching it's awake
			// until the time since the last processing start is greater
			// than or equal to the engine's time delta
			auto timeSinceLastLoop = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - this->lastMomentProcessingStarted).count();
			auto timeBetweenLoops = int(this->timeDelta * 1000);
			if (timeSinceLastLoop < (timeBetweenLoops)) {
				char buffer[100];
				sprintf_s(buffer, "sleeping for: %d\n", timeBetweenLoops - timeSinceLastLoop);
				OutputDebugStringA(buffer);
				Sleep(timeBetweenLoops - timeSinceLastLoop);
			}
		}
	}
}

void PhysicsEngine::performAddCurrentBodiesToGrid()
{
	std::lock_guard<std::mutex> lock(bodiesAccessLock);
	this->bodiesGrid.addBodiesToGrid(this->bodiesToAddToGrid);
	this->bodiesToAddToGrid.clear();
}

void PhysicsEngine::runUpdateBatch(int threadIndex) {

	this->runFunctionOverThreadBodies(threadIndex, [&](Body& body, int index) {
		auto surroundingBodies = this->bodiesGrid.getBodySourroundingBodiesVectors(index);
		body.calculateInteractionWithBodies(surroundingBodies);
	});
	int x = 0;
	for (int i = 0; i < 100000; i++)
	{
		x += sqrt(3.14 * i);
	}
	char buffer[100];
	sprintf_s(buffer, "runUpdateBatch: %d\n", x);
	OutputDebugStringA(buffer);
}

void PhysicsEngine::applyUpdates(int threadIndex) {
	this->runFunctionOverThreadBodies(threadIndex, [&](Body& body, int index) {
		body.applyInteraction();
	});
	int x = 0;
	for (int i = 0; i < 10000; i++)
	{
		x += sqrt(3.14 * i);
	}
	char buffer[100];
	sprintf_s(buffer, "applyUpdates: %d\n", x);
	OutputDebugStringA(buffer);
}

template<typename T> void PhysicsEngine::runFunctionOverThreadBodies(int threadIndex, T&& func) {
	const int bodiesPerThread = this->totalBodiesForProcessingLoop / this->runningThreads;

	int bodiesForCurrentThreadStartIndex = bodiesPerThread * threadIndex;

	int bodiesForCurrentThreadEndIndex = (bodiesPerThread * (threadIndex + 1)) - 1;

	if (threadIndex == this->runningThreads - 1) {
		bodiesForCurrentThreadEndIndex += this->totalBodiesForProcessingLoop % this->runningThreads;
	}

	for (int i = bodiesForCurrentThreadStartIndex; i <= bodiesForCurrentThreadEndIndex; i++)
	{
		Body& body = this->bodiesGrid.getBodyAtIndex(i);
		func(body, i);
	}
}
