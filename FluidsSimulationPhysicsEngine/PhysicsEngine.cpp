#include "PhysicsEngine.h"
#include <windows.h>

void* engineUpdateLoopThreadFunction(void * engineArg, int threadIndex)
{
	((PhysicsEngine*)engineArg)->engineUpdateLoop(threadIndex);
	return 0;
}

PhysicsEngine::~PhysicsEngine() {
	this->shouldStopEngine = true;
	for (int i = 0; i < this->runningThreads; i++)
	{
		if (this->engineUpdateLoopThreads[i]->joinable())
		{
			this->engineUpdateLoopThreads[i]->join();
		}
		delete this->engineUpdateLoopThreads[i];
	}

	delete this->engineUpdateLoopThreads;

}

PhysicsEngine::PhysicsEngine()
{
	this->shouldStopEngine = false;
	this->shouldRunLoop = false;
	this->runningThreads = max(1, thread::hardware_concurrency());

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

		threadsProcessingCurrentLoop++;

		this->runUpdateBatch(threadIndex);

		// main thread
		if (threadIndex == 0) {

			{
				// wait until all threads besides this one finish processing
				std::unique_lock<std::mutex> lock(shouldBeProcessingNextUpdateLoopLocker);
				shouldBeProcessingNextUpdateLoopConditional.wait(lock, [&] {return threadsProcessingCurrentLoop == 1; });
			}
			this->applyUpdates();
		}

		threadsProcessingCurrentLoop--;

		{
			// notify all other threads that the current one is finished
			std::lock_guard<std::mutex> lock(shouldBeProcessingNextUpdateLoopLocker);
			shouldBeProcessingNextUpdateLoopConditional.notify_all();
		}

		{
			// wait until all other threads finish processing
			std::unique_lock<std::mutex> lock(shouldBeProcessingNextUpdateLoopLocker);
			shouldBeProcessingNextUpdateLoopConditional.wait(lock, [&] {return threadsProcessingCurrentLoop == 0; });
		}

		
	}
}

void PhysicsEngine::runUpdateBatch(int threadIndex) {

}

void PhysicsEngine::applyUpdates() {

}
