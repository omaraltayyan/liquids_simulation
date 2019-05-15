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

	const int calculationOperationsCount = 3;

	int totalBodiesForProcessingLoop;

	int totalBarriers;

	void engineUpdateLoop(int threadIndex);

	atomic<bool> lockGridAddition;

	mutex bodiesAccessLock;
	mutex bodiesToAddToGridLock;
	BodiesVector bodiesToAddToGrid;

	int runningThreads;

	void runUpdateBatch(int threadIndex, int calculationOperation);
	
	void applyUpdates(int threadIndex);

	chrono::steady_clock::time_point lastMomentProcessingStarted;

	Grid bodiesGrid;
	
	template<typename T> void runFunctionOverThreadBodies(int threadIndex, T&& func);
	
	void performAddCurrentBodiesToGrid();

	static PhysicsEngine* sharedEngine;

public:

	static PhysicsEngine* shared();

	qreal timeDelta = 1.0 / 64.0;

	void resumeEngine();
	void pauseEngine();

	int getBodiesCount();

	void addBodiesToGrid(BodiesVector);

	template<typename T> void runFunctionOverBodies(T&& func) {
		std::lock_guard<std::mutex> lock(bodiesAccessLock);
		auto bodies = this->bodiesGrid.getAllBodies();
		for (int i = 0; i < this->bodiesGrid.bodiesCount(); i++)
		{
			func(this->bodiesGrid.getBodyAtIndex(i));
		}
	};

	// WARNING: not thread safe, don't access the bodies or 
	// add bodies them to the grid, use runFunctionOverBodies
	// and addBodiesToGrid respectively instead
	Grid& getUnsafeBodiesGrid();

	PhysicsEngine();
	~PhysicsEngine();
};
