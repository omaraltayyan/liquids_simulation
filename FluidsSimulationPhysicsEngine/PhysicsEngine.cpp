#include "PhysicsEngine.h"
#include <windows.h>

void* engineUpdateLoopThreadFunction(void * engineArg, int threadIndex)
{
	((PhysicsEngine*)engineArg)->engineUpdateLoop(threadIndex);
	return 0;
}

PhysicsEngine::~PhysicsEngine() {
	this->shouldStopEngine = true;
	processingStartSyncronizationBarrier->Break();
	processingEndSyncronizationBarrier->Break();
	for (int i = 0; i < this->runningThreads; i++)
	{
		if (this->engineUpdateLoopThreads[i]->joinable())
		{
			this->engineUpdateLoopThreads[i]->join();
		}
		delete this->engineUpdateLoopThreads[i];
	}

	delete this->engineUpdateLoopThreads;
	delete processingStartSyncronizationBarrier;
	delete processingEndSyncronizationBarrier;

}

PhysicsEngine::PhysicsEngine()
{
	this->lastMomentProcessingStarted = chrono::high_resolution_clock::now();
	this->shouldStopEngine = false;
	this->shouldRunLoop = false;
	this->runningThreads = max(1, thread::hardware_concurrency());
	processingStartSyncronizationBarrier = new ThreadsBarrier(runningThreads);
	processingEndSyncronizationBarrier = new ThreadsBarrier(runningThreads);

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

		if (threadIndex == 0) {
			this->lastMomentProcessingStarted = chrono::high_resolution_clock::now();
		}

		this->runUpdateBatch(threadIndex);

		processingStartSyncronizationBarrier->Await();

		// stop the loop when exiting the thread is required
		if (shouldStopEngine) {
			break;
		}

		if (threadIndex == 0) {

			this->applyUpdates();

			// main thread stalls others by not raising it's counter
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
		processingEndSyncronizationBarrier->Await();

	}
}

void PhysicsEngine::runUpdateBatch(int threadIndex) {
	int sum = 0;
	for (int i = 0; i < 1000; i++)
	{
		sum += i;
	}
	char buffer[100];
	sprintf_s(buffer, "runUpdateBatch: %d thread %d\n", sum, threadIndex);
	OutputDebugStringA(buffer);
}

void PhysicsEngine::applyUpdates() {
	int sum = 0;
	for (int i = 0; i < 1000; i++)
	{
		sum += i;
	}
	char buffer[100];
	sprintf_s(buffer, "applyUpdates: %d\n", sum);
	OutputDebugStringA(buffer);
}
