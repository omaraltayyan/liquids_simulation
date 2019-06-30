#pragma once

#include <random>
#include "PhysicsEngine.h"
#include <Material.h>


class RandomEmitter
{
	std::random_device randomDevice;
	std::mt19937 randomEngine;

public:
	void addRandomBodies(PhysicsEngine* engine, int bodiesCount = -1, QPointF generationPosition = QPointF());
	double emittedParticleRadius = 1e-10;
	double particlesPerEmission = 10;
	double emissionAreaSquareRadius = 0.03;

	Material currentMaterial = water;

	// water
	//double emittedParticleViscosity = 3.5;
	//double emittedParticleMass = 0.02;
	//double emittedParticleSurfaceTension = 0.0728;
	//double emittedParticleThreshold = 7.065;
	//double emittedParticleGasConstant = 3;
	//double emittedParticleRestDensity = 998.29;
	//double emittedParticleRestitution = 0;
	//double emittedParticleBuoyancy = 0;
	//QColor emittedParticleColor = QColor(173, 216, 230);


	// mucus
	//double emittedParticleViscosity = 36;
	//double emittedParticleMass = 0.04;
	//double emittedParticleSurfaceTension = 6;
	//double emittedParticleThreshold = 5;
	//double emittedParticleGasConstant = 5;
	//double emittedParticleRestDensity = 1000;
	//double emittedParticleRestitution = 0.5;
	//double emittedParticleBuoyancy = 0;


	// steam
	//double emittedParticleViscosity = 0.01;
	//double emittedParticleMass = 5e-5;
	//double emittedParticleSurfaceTension = 0;
	//double emittedParticleThreshold = 1e300;
	//double emittedParticleGasConstant = 4;
	//double emittedParticleRestDensity = 0.59;
	//double emittedParticleRestitution = 0;
	//double emittedParticleBuoyancy = 5;

	RandomEmitter();
	~RandomEmitter();
};

