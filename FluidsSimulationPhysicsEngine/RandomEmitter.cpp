#include "RandomEmitter.h"
#include "Bodies\FluidParticle.h"
#include "PhysicsEngine.h"
#include <iostream>
#include <stdio.h>


void RandomEmitter::addRandomBodies(PhysicsEngine* engine, int bodiesCount, QPointF generationPosition) {

	auto gridSize = engine->getUnsafeBodiesGrid().sizeInMeters();

	if (generationPosition.isNull()) {
		generationPosition = QPointF(gridSize.width() / 2, gridSize.height() / 2);
	}
	if (generationPosition.x() < 0 || generationPosition.x() >= gridSize.width() ||
		generationPosition.y() < 0 || generationPosition.y() >= gridSize.height()) {
		return;
	}

	/*std::uniform_real_distribution<> XPositionDistribution(
		generationPosition.x() - this->emissionAreaSquareRadius,
		generationPosition.x() + this->emissionAreaSquareRadius);

	std::uniform_real_distribution<> YPositionDistribution(
		generationPosition.y() - this->emissionAreaSquareRadius,
		generationPosition.y() + this->emissionAreaSquareRadius);*/

	if (bodiesCount == -1) {
		bodiesCount = this->particlesPerEmission;
	}
	BodiesVector bodies;
	bodies.reserve(bodiesCount);
	int xMargin = 0; int yMargin = 0;
	auto drawingPosition = generationPosition;
	for (int i = 0; i < bodiesCount; i++)
	{
		
		//auto position = QPointF(XPositionDistribution(randomEngine), YPositionDistribution(randomEngine));

		drawingPosition.setX(drawingPosition.x() - (xMargin*emissionAreaSquareRadius));
		drawingPosition.setY(generationPosition.y() - (yMargin*emissionAreaSquareRadius));
		yMargin = 1 - yMargin;
		auto position = QPointF(drawingPosition.x(), drawingPosition.y());
		
		auto body = new FluidParticle(position, engine, emittedParticleViscosity,
			emittedParticleMass, emittedParticleGasConstant, emittedParticleRestDensity,
			emittedParticleSurfaceTension, emittedParticleThreshold, emittedParticleRestitution, emittedParticleBuoyancy,
			emittedParticleColor);

		bodies.push_back(body);
		if (yMargin == 0)
			xMargin = 1;
		else
			xMargin = 0;
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
