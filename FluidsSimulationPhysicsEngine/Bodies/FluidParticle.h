#pragma once

#include "Particle.h"
#include <QVector2D>
#include <qmath.h>
#include "Grid.h"
#include "Utilities\MathUtilities.h"
#include <functional>

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
		double surfaceTension, double threshold, double restitution);

	void calculateInteractionWithBodies(const QVector<BodiesVector*>& surroundingBodies, int calculationOperation) override;
	void applyInteraction() override;

	double displayRadius;
private:
	bool _isFirstIteration;
	double _density, _viscosity, _pressure, _mass, _gasConstant, _restDensity, _tensionCoefcioant, _surfaceThreshold, _restitution;
	QVector2D _velocity,_accelration, _leapFrogNextStep, _force, _leapFrogPreviousStep;

	double applyKernal(double distance, SmoothingKernals kernal);
	double computeDensity(const QVector<BodiesVector*>& surroundingBodies, double radius);
	double computePressure();
	QVector2D computePressureForce(const QVector<BodiesVector*>& surroundingBodies, double radius);
	QVector2D computeViscousForce(const QVector<BodiesVector*>& surroundingBodies, double radius);
	QVector2D computeSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius);
	double computeLaplacianSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius);
	QVector2D computeSurfaceTension(const QVector<BodiesVector*>& surroundingBodies, double radius);

	void runFunctionOverFluidParicles(const QVector<BodiesVector*>& surroundingBodies, double radius, const std::function <void(FluidParticle*, double)>&& func);
	QVector2D computeSumOfForces(const QVector<BodiesVector*>& surroundingBodies, double radius);

	void applyLeapFrogTimeStepIntegration();
	void detectCollision(const QRectF& boundingBox);
	int signumFunction(double x);


};

