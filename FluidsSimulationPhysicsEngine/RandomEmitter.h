#pragma once

#include <random>
#include "PhysicsEngine.h"


class RandomEmitter
{
	std::random_device randomDevice;
	std::mt19937 randomEngine;

public:
	void addRandomBodies(PhysicsEngine* engine, int bodiesCount, QPointF generationPosition = QPointF());
	double emittedParticleRadius = 0.01;
	double emittedParticleViscosity = 3.5;
	double emittedParticleMass = 0.02;
	double emittedParticleSurfaceTension = 0.0728;
	double emittedParticleThreshold = 7.065;
	double emittedParticleGasConstant = 0.03;
	double emittedParticleRestDensity = 998.29;
	double particlesPerEmission = 4;
	
	
	qreal emissionAreaSquareRadius = 0.01;

	RandomEmitter();
	~RandomEmitter();
};

