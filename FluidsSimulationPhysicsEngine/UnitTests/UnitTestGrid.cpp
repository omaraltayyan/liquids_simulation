#include "UnitTestGrid.h"
#include "Grid.h"
#include "Bodies\FluidParticle.h"
#include "PhysicsEngine.h"
#include <QRectF>
#include <windows.h>
#include <iostream>
#include <stdio.h>


UnitTestGrid::UnitTestGrid() : randomEngine(randomDevice())
{
	randomEngine.seed();
}


UnitTestGrid::~UnitTestGrid()
{
}

void UnitTestGrid::addRandomBodies() {

	auto e = PhysicsEngine::shared();
	auto gridSize = e->getUnsafeBodiesGrid().sizeInMeters();

	std::normal_distribution<> XPositionDistribution(0, gridSize.width());
	std::normal_distribution<> YPositionDistribution(0, gridSize.height());

	BodiesVector bodies;
	bodies.reserve(5000);
	for (int i = 0; i < 5000; i++)
	{
		auto position = QPointF(XPositionDistribution(randomEngine), YPositionDistribution(randomEngine));
		auto body = new FluidParticle(position, 0.2, 0.2, 0.2);
		bodies.push_back(body);
	}
	e->addBodiesToGrid(bodies);
}

void UnitTestGrid::run() {
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	//auto grid = Grid(QSize(24, 18), 3);
	//auto body1 = new Body(QRectF(1, 1, 4, 5));
	//auto body2 = new Body(QRectF(12, 10, 1, 2));
	//auto body3 = new Body(QRectF(4, 2.9, 2.2, 1.2));

	//BodiesVector bodies = { body1, body2,  body3 };
	//grid.addBodiesToGrid(bodies);
	//printf("tests valid");
	auto e = PhysicsEngine::shared();
	
	addRandomBodies();

	e->resumeEngine();
	addRandomBodies();
	Sleep(5000);

	e->pauseEngine();
	addRandomBodies();

	printf("paused\n");
	Sleep(5000);
	e->resumeEngine();

	addRandomBodies();

	Sleep(5000);
	e->pauseEngine();

	printf("paused\n");
	addRandomBodies();

	Sleep(5000);
	e->resumeEngine();

	addRandomBodies();

	printf("tests valid\n");
	Sleep(1000000);
}
