#pragma once

#include <random>
#include "PhysicsEngine.h"


class RandomEmitter
{
	std::random_device randomDevice;
	std::mt19937 randomEngine;

public:
	void addRandomBodies(PhysicsEngine* engine, int bodiesCount, QPointF generationPosition = QPointF());

	qreal squareRadius = 2;

	RandomEmitter();
	~RandomEmitter();
};

