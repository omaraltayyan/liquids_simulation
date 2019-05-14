#pragma once
#include <QRect>
#include <QVector2D>
#include <QVector>

class Body;

typedef QVector<Body*> BodiesVector;

class Body
{
public:
	virtual Body* drawableClone() const = 0;

	virtual void draw() const = 0;

	virtual void calculateInteractionWithBodies(QVector<BodiesVector*> surroundingBodies, int calculationOperation) {}
	virtual void applyInteraction() {}

	QRectF boundingRect;

	Body();
	~Body();
};

