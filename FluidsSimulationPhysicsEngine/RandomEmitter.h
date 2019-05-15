#pragma once

#include <random>
#include "PhysicsEngine.h"


class RandomEmitter
{
	std::random_device randomDevice;
	std::mt19937 randomEngine;

public:
	void addRandomBodies(PhysicsEngine* engine, int bodiesCount, QPointF generationPosition = QPointF());
	double emittedParticleRadius = 0.1;
	double emittedParticleViscosity = 30;
	double emittedParticleMass = 65;
	double emittedParticleGasConstant = 0.01;
	double emittedParticleRestDensity = 2000;
		
	qreal emissionAreaSquareRadius = 2;

	RandomEmitter();
	~RandomEmitter();
};
