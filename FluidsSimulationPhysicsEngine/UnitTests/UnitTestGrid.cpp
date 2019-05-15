#include "UnitTestGrid.h"
#include "Grid.h"
#include "Bodies\FluidParticle.h"
#include "PhysicsEngine.h"
#include "RandomEmitter.h"
#include <QRectF>
#include <windows.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>

UnitTestGrid::UnitTestGrid()
{
}

UnitTestGrid::~UnitTestGrid()
{
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
	
	RandomEmitter emitter;

	emitter.addRandomBodies(e, 5000);

	e->resumeEngine();
	emitter.addRandomBodies(e, 5000);
	Sleep(5000);

	e->pauseEngine();
	emitter.addRandomBodies(e, 5000);

	printf("paused\n");
	Sleep(1000);
	e->resumeEngine();

	emitter.addRandomBodies(e, 5000);

	Sleep(5000);
	e->pauseEngine();

	printf("paused\n");
	emitter.addRandomBodies(e, 5000);

	Sleep(1000);
	e->resumeEngine();

	emitter.addRandomBodies(e, 5000);

	printf("tests valid\n");
	Sleep(5000);

	Sleep(1000000);
}
