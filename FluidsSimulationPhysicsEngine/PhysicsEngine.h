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

	const int constantBarriersBeforeCalculationsCount = 2;

	const int constantBarriersCount = 3;

	const int calculationOperationsCount = 2;

	int totalBodiesForProcessingLoop;

	int totalBarriers;

	void engineUpdateLoop(int threadIndex);

	atomic<bool> lockGrid;

	mutex bodiesAccessLock;
	BodiesVector bodiesToAddToGrid;

	int runningThreads;

	void runUpdateBatch(int threadIndex, int calculationOperation);
	
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
