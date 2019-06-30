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
	double radius = 1e-10;
	double particlesPerEmission = 10;
	double emissionAreaSquareRadius = 0.03;

	Material currentMaterial = water;

	// water
	//double viscosity = 3.5;
	//double mass = 0.02;
	//double surfaceTension = 0.0728;
	//double threshold = 7.065;
	//double gasConstant = 3;
	//double restDensity = 998.29;
	//double restitution = 0;
	//double buoyancy = 0;
	//QColor color = QColor(173, 216, 230);


	// mucus
	//double viscosity = 36;
	//double mass = 0.04;
	//double surfaceTension = 6;
	//double threshold = 5;
	//double gasConstant = 5;
	//double restDensity = 1000;
	//double restitution = 0.5;
	//double buoyancy = 0;


	// steam
	//double viscosity = 0.01;
	//double mass = 5e-5;
	//double surfaceTension = 0;
	//double threshold = 1e300;
	//double gasConstant = 4;
	//double restDensity = 0.59;
	//double restitution = 0;
	//double buoyancy = 5;

	RandomEmitter();
	~RandomEmitter();
};

