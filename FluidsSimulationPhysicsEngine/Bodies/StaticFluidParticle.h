#pragma once
#include "Bodies\FluidParticle.h"
class StaticFluidParticle :
	public FluidParticle
{
public:

	QCPVector2D StaticFluidParticle::computeSumOfForces(const QVector<BodiesVector*>& surroundingBodies, double radius) override;
	void StaticFluidParticle::applyInteraction() override;

	StaticFluidParticle(const QPointF& position, PhysicsEngine* engine,
		double viscosity, double mass, double gasConstant, double restDesity,
		double surfaceTension, double threshold, double restitution, double buoyancy, QColor color, double radius);
};

