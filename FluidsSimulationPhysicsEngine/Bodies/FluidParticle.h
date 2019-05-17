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
		double viscosity, double mass, double gasConstant, double restDesity,double _velocityHalfStep,
		double surfaceTension, double threshold);

	void calculateInteractionWithBodies(const QVector<BodiesVector*>& surroundingBodies, int calculationOperation) override;
	void applyInteraction() override;

private:
	double _density, _viscosity, _pressure, _mass, _gasConstant, _restDensity, _tensionCoefcioant, _surfaceThreshold;
	QVector2D _velocity, _force, _tempVelocity, _tempForce;


	double applyKernal(double distance, double radius, SmoothingKernals kernal);
	double computeDynsity(const QVector<FluidParticle*>& fuildParticles, double radius);
	double computePressure(double gasConstant, double restDynsity, double density);
	QVector2D computePressureForce(const QVector<FluidParticle*>& fuildParticles, double radius);
	QVector2D computeViscousForce(const QVector<FluidParticle*>& fuildParticles, double radius);
	QVector2D computeSurfaceNormal(const QVector<FluidParticle*>& fuildParticles, double radius);
	double computeLaplacianSurfaceNormal(const QVector<FluidParticle*>& fuildParticles, double radius);
	QVector2D computeSurfaceTension(const QVector<FluidParticle*>& fuildParticles, double radius);
	QVector<FluidParticle*> filterFuildParticles(const QVector<BodiesVector*>& surroundingBodies, double radius);
	QVector2D computeSumOfForces(const QVector<FluidParticle*>& fluidParticles, double radius);


};

