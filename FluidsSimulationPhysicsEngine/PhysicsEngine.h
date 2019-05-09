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


	friend void* engineUpdateLoopThreadFunction(void * engineArg, int threadIndex);

	atomic<bool> shouldRunLoop;
	atomic<bool> shouldStopEngine;

	condition_variable shouldBeProcessingNextUpdateLoopConditional;
	mutex shouldBeProcessingNextUpdateLoopLocker;

	ThreadsBarrier** synchronizationBarriers;

	const int barriersCount = 5;

	int totalBodiesForProcessingLoop;

	void engineUpdateLoop(int threadIndex);

	atomic<bool> lockGrid;

	mutex bodiesAccessLock;
	BodiesVector bodiesToAddToGrid;

	int runningThreads;

	void runUpdateBatch(int threadIndex);
	
	void applyUpdates(int threadIndex);

	chrono::steady_clock::time_point lastMomentProcessingStarted;

	Grid bodiesGrid;
	
	template<typename T> void runFunctionOverThreadBodies(int threadIndex, T&& func);
	
	void performAddCurrentBodiesToGrid();
public:

	qreal timeDelta = 1.0 / 64.0;

	void resumeEngine();
	void pauseEngine();

	void addBodiesToGrid(BodiesVector);

	PhysicsEngine();
	~PhysicsEngine();
};
