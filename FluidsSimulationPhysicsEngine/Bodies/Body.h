#pragma once
#include <QRect>
#include <QVector2D>
#include <QVector>

class Body;

typedef QVector<Body*> BodiesVector;

class Body
{
public:

	virtual void draw() = 0;

	virtual void calculateInteractionWithBodies(QVector<BodiesVector*> surroundingBodies, int calculationOperation) {}
	virtual void applyInteraction() {}

	QRectF boundingRect;

	Body();
	~Body();
};

