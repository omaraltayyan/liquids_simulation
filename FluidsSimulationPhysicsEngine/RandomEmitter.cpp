#include "RandomEmitter.h"
#include "Bodies\FluidParticle.h"
#include "PhysicsEngine.h"
#include <iostream>
#include <stdio.h>


void RandomEmitter::addRandomBodies(PhysicsEngine* engine, int bodiesCount, QPointF generationPosition) {

	auto gridSize = engine->getUnsafeBodiesGrid().sizeInCentimeters();

	if (generationPosition.isNull()) {
		generationPosition = QPointF(gridSize.width() / 2, gridSize.height() / 2);
	}
	if (generationPosition.x() < 0 || generationPosition.x() >= gridSize.width() ||
		generationPosition.y() < 0 || generationPosition.y() >= gridSize.height()) {
		return;
	}

	std::uniform_real_distribution<> XPositionDistribution(generationPosition.x() - squareRadius, generationPosition.x() + squareRadius);
	std::uniform_real_distribution<> YPositionDistribution(generationPosition.y() - squareRadius, generationPosition.y() + squareRadius);

	BodiesVector bodies;
	bodies.reserve(bodiesCount);
	for (int i = 0; i < bodiesCount; i++)
	{
		auto position = QPointF(XPositionDistribution(randomEngine), YPositionDistribution(randomEngine));
		auto body = new FluidParticle(position, 0.2, 250, 65);
		bodies.push_back(body);
	}
	engine->addBodiesToGrid(bodies);
}

RandomEmitter::RandomEmitter() : randomEngine(randomDevice())
{
	randomEngine.seed();
}


RandomEmitter::~RandomEmitter()
{
}
