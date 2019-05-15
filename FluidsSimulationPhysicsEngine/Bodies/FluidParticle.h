#pragma once

#include "Particle.h"
#include <QVector2D>
#include <qmath.h>
#include "Grid.h"
#include "PhysicsEngine.h"

enum SmoothingKernals
{
	poly6,
	spiky,
	visc
};
class FluidParticle : public Particle
{
public:

	Body drawableClone() const {
		return *this;
	};

	FluidParticle(const QPointF& position, qreal sizeRadius, double viscosity, double mass);

private:
	double _dynsity, _viscosity, _pressure, _mass;
	QVector2D _velocity, _force, _tempVelocity, _tempForce;
	double applyKernal(double distance, double radius, SmoothingKernals kernal);
	double computeDynsity(QVector<BodiesVector*> surroundingBodies, double radius);
	double computePressure(double gasConstant, double restDynsity, double dynsity);
	QVector2D computePressureForce(QVector<BodiesVector*> surroundingBodies, double radius);
	QVector2D computeViscousForce(QVector<BodiesVector*> surroundingBodies, double radius);
	QVector2D computeSumOfForces(QVector<BodiesVector*> surroundingBodies, double radius, double gravityValue);
	QVector2D computeVelocityChange(double deltaTime,QVector2D sumForces);

	void calculateInteractionWithBodies(QVector<BodiesVector*> surroundingBodies, int calculationOperation);
	void applyInteraction();
	
	 
};

