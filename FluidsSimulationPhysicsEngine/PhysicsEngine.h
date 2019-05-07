#pragma once

#include "fluidssimulationphysicsengine_global.h"
#include "qrect.h"
#include "ThreadsBarrier.h"
#include "Grid.h"
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
	ThreadsBarrier* processingAfterUpdateSyncronizationBarrier;
	
	int totalBodiesForProcessingLoop;

	void engineUpdateLoop(int threadIndex);

	int runningThreads;

	void runUpdateBatch(int threadIndex);
	
	void applyUpdates(int threadIndex);

	chrono::steady_clock::time_point lastMomentProcessingStarted;

	Grid bodiesGrid;
	
	template<typename T> void runFunctionOverThreadBodies(int threadIndex, T&& func);

public:

	qreal timeDelta = 1.0 / 64.0;

	void resumeEngine();
	void pauseEngine();

	PhysicsEngine();
	~PhysicsEngine();
};
