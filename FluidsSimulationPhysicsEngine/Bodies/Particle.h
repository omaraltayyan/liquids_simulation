#pragma once
#include "MovingBody.h"
#include "QCPVector2D.h"

class Particle :
	public MovingBody
{
	void updatePosition();

public:

	QPointF position;
	QCPVector2D positionVector;
	void setPosition(const QPointF& newPosition);
	void setPosition(qreal x, qreal y);

	Particle(const QPointF& position, PhysicsEngine* engine);
};

