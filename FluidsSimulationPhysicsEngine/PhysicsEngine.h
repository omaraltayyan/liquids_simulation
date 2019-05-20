#pragma once

#include "fluidssimulationphysicsengine_global.h"
#include "qrect.h"
#include "ThreadsBarrier.h"
#include "Grid.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "QCPVector2D.h"

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
	QVector<double> frameTimeSamples;

	const int fpsAverageSamples = 60;

	const int calculationOperationsCount = 2;

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
	
	void runFunctionOverThreadBodies(int threadIndex, const function <void(Body*, int)>&& func);
	
	void performAddCurrentBodiesToGrid();

	static PhysicsEngine* sharedEngine;

	atomic<bool> clearBoddies = false;
	atomic<QCPVector2D*> newGravity;
	QCPVector2D gravity = QCPVector2D(0.0, 9.8);

	void perfromClearBoddies();
	
	qreal timeDelta = 0.001;
	atomic<qreal> newTimeDelta;

public:

	static PhysicsEngine* shared();

	void setTimeDelta(qreal newTimeDelta);
	qreal getTimeDelta();

	atomic<int> bodiesCount = 0;
	atomic<double> fps = 0;
	void resumeEngine();
	void pauseEngine();

	bool isPaused();

	atomic<int> speedSlownessScale = 1;

	int getBodiesCount();

	void clearBodies();


	void addBodiesToGrid(BodiesVector);

	void runFunctionOverBodies(const std::function <void(Body*)>&& func);
	
	void setGravity(const QCPVector2D& newGravity);
	const QCPVector2D& getGravity();

	// WARNING: not thread safe, don't access the bodies or 
	// add bodies them to the grid, use runFunctionOverBodies
	// and addBodiesToGrid respectively instead
	Grid& getUnsafeBodiesGrid();

	PhysicsEngine();
	~PhysicsEngine();
};
