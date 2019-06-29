#include "StaticFluidParticle.h"

StaticFluidParticle::StaticFluidParticle(const QPointF& position, PhysicsEngine* engine,
	double viscosity, double mass, double gasConstant, double restDesity,
	double surfaceTension, double threshold, double restitution, double buoyancy, QColor color) : FluidParticle(position, engine,
		 viscosity,  mass,  gasConstant,  restDesity,
		 surfaceTension,  threshold,  restitution,  buoyancy, color) {
	this->displayRadius = 0;
}

QCPVector2D StaticFluidParticle::computeSumOfForces(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	return QCPVector2D(0, 0);
}

void StaticFluidParticle::applyInteraction() {

}