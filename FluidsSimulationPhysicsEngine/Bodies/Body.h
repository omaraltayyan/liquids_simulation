#pragma once
#include <QRect>
#include <QVector2D>
#include <QVector>

class Body;
class PhysicsEngine;

typedef QVector<Body*> BodiesVector;

class Body
{
public:
	
	PhysicsEngine* engine;

	virtual void calculateInteractionWithBodies(const QVector<BodiesVector*>& surroundingBodies, int calculationOperation) {}
	virtual void applyInteraction() {}

	QRectF boundingRect;

	Body(const Body& p);
	Body(QRectF boundingRect, PhysicsEngine* engine);
	Body();
};

