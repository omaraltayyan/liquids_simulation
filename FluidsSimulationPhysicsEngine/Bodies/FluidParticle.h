#pragma once

#include "Particle.h"
#include <QVector2D>
#include <qmath.h>
#include "Grid.h"

enum SmoothingKernals
{
	poly6,
	gradPoly6,
	lapPoly6,
	spiky,
	visc
};
class FluidParticle : public Particle
{
public:

	FluidParticle(const QPointF& position, PhysicsEngine* engine, qreal sizeRadius,
		double viscosity, double mass, double gasConstant, double restDesity,
		double surfaceTension, double threshold);

private:
	double _density, _viscosity, _pressure, _mass, _gasConstant, _restDensity, _tensionCoefcioant, _surfaceThreshold;
	QVector2D _velocity, _force, _tempVelocity, _tempForce;


	double applyKernal(double distance, double radius, SmoothingKernals kernal);
	double computeDynsity(QVector<BodiesVector*> surroundingBodies, double radius);
	double computePressure(double gasConstant, double restDynsity, double density);
	QVector2D computePressureForce(QVector<BodiesVector*> surroundingBodies, double radius);
	QVector2D computeViscousForce(QVector<BodiesVector*> surroundingBodies, double radius);
	QVector2D computeSurfaceNormal(QVector<BodiesVector*> surroundingBodies, double radius);
	double computeLaplacianSurfaceNormal(QVector<BodiesVector*> surroundingBodies, double radius);
	QVector2D computeSurfaceTension(QVector<BodiesVector*> surroundingBodies, double radius);
	QVector2D computeSumOfForces(QVector<BodiesVector*> surroundingBodies, double radius);
	QVector2D computeVelocityChange(double deltaTime, QVector2D sumForces);

	void calculateInteractionWithBodies(QVector<BodiesVector*> surroundingBodies, int calculationOperation);
	void applyInteraction();


};

