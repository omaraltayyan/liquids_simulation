#pragma once

#include "fluidssimulationphysicsengine_global.h"
#include "qrect.h"
#include "ThreadsBarrier.h"
#include <thread>
#include <atomic>
#include <mutex>
using namespace std;

class FLUIDSSIMULATIONPHYSICSENGINE_EXPORT PhysicsEngine
{
	thread** engineUpdateLoopThreads;

	mutex bodiesAccessLock;

	friend void* engineUpdateLoopThreadFunction(void * engineArg, int threadIndex);

	atomic<bool> shouldRunLoop;
	atomic<bool> shouldStopEngine;

	condition_variable shouldBeProcessingNextUpdateLoopConditional;
	mutex shouldBeProcessingNextUpdateLoopLocker;

	ThreadsBarrier* processingStartSyncronizationBarrier;
	ThreadsBarrier* processingEndSyncronizationBarrier;

	void engineUpdateLoop(int threadIndex);

	int runningThreads;

	void runUpdateBatch(int threadIndex);
	
	void applyUpdates();

	chrono::steady_clock::time_point lastMomentProcessingStarted;

public:

	qreal timeDelta = 1.0 / 64.0;

	void resumeEngine();
	void pauseEngine();

	PhysicsEngine();
	~PhysicsEngine();
};
