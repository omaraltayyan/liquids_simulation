#pragma once
#include "MovingBody.h"
class Particle :
	public MovingBody
{
	void updatePosition();

public:

	qreal sizeRadius;
	QPointF position;
	QVector2D positionVector;
	void setPosition(const QPointF& newPosition);
	void setPosition(qreal x, qreal y);

	Particle(const QPointF& position, qreal sizeRadius);
};

