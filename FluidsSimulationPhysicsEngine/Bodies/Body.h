#pragma once
#include <QRect>
#include <QVector2D>
#include <QVector>

class Body;

typedef QVector<Body*> BodiesVector;

class Body
{
public:
	virtual Body drawableClone() const {
		return *this;
	};

	virtual void calculateInteractionWithBodies(QVector<BodiesVector*> surroundingBodies, int calculationOperation) {}
	virtual void applyInteraction() {}

	QRectF boundingRect;

	Body(const Body& p);
	Body(QRectF boundingRect);
	Body();
};

