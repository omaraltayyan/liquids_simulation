#pragma once

#include <random>
#include "PhysicsEngine.h"


class RandomEmitter
{
	std::random_device randomDevice;
	std::mt19937 randomEngine;

public:
	void addRandomBodies(PhysicsEngine* engine, int bodiesCount, QPointF generationPosition = QPointF());
	double emittedParticleRadius = 1;
	double emittedParticleViscosity = 400;
	double emittedParticleMass = 65;
	double emittedParticleGasConstant = 0.01;
	double emittedParticleRestDensity = 1000;
	double particlesPerEmission = 20;
	
	
	qreal emissionAreaSquareRadius = 1;

	RandomEmitter();
	~RandomEmitter();
};

