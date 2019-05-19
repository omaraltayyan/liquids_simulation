#include "FluidParticle.h"
#include "PhysicsEngine.h"
#include <qmath.h>

FluidParticle::FluidParticle(const QPointF& position, PhysicsEngine* engine, qreal sizeRadius,
	double viscosity, double mass, double gasConstant, double restDesity,
	double surfaceTension, double threshold, double restitution) : Particle(position, engine, sizeRadius)
{
	_restitution = restitution;
	_viscosity = viscosity;
	_mass = mass;
	_gasConstant = gasConstant;
	_restDensity = restDesity;
	_velocity.setX(0.0);
	_velocity.setY(0.0);

	_accelration.setX(0.0);
	_accelration.setY(0.0);

	_tensionCoefcioant = surfaceTension;
	_surfaceThreshold = threshold;
	_isFirstIteration = true;
	this->bodyType = fluid;
	this->displayRadius = cbrt(3 * this->_mass / (4 * M_PI * this->_restDensity));
}


double FluidParticle::applyKernal(double distance, SmoothingKernals kernal)
{
	const KernelsInfo& kernelsInfo = this->engine->getUnsafeBodiesGrid().getKernelsInfo();

	if (kernal == poly6 || kernal == gradPoly6 || kernal == lapPoly6) {
		double holder = kernelsInfo.radius_2 - (distance * distance);
		switch (kernal)
		{
		case poly6:
			return kernelsInfo.poly6Left * (holder*holder*holder);
		case gradPoly6:
			return kernelsInfo.gradAndLapPoly6Left * (holder*holder);
		case lapPoly6:
			return kernelsInfo.gradAndLapPoly6Left * holder * ((3 * kernelsInfo.radius_2) - (7 * distance*distance));
		}
	}

	switch (kernal)
	{
	case spiky:
		return kernelsInfo.spikyLeft * ((kernelsInfo.radius - distance) * (kernelsInfo.radius - distance));
	case visc:
		return kernelsInfo.viscLeft * (kernelsInfo.radius - distance);
	}

	return 0.0;


}

double FluidParticle::computeDensity(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	double resultingDynsity = 0.0;
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		resultingDynsity += particle->_mass * this->applyKernal(distance, poly6);
	});
	return resultingDynsity;
}

double FluidParticle::computePressure()
{
	return _gasConstant  * (_density - _restDensity);
}

QCPVector2D FluidParticle::computePressureForce(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingPressureForce(0.0, 0.0);
	auto relativePressureTerm = this->_pressure / (this->_density * this->_density);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this || MathUtilities::isEqual(distance, 0))
			return;
		auto vec = this->positionVector - particle->positionVector;
		//vec.normalize();
		auto leftTerm = (relativePressureTerm + (particle->_pressure / (particle->_density * particle->_density))) * particle->_mass;
		resultingPressureForce += vec * leftTerm * this->applyKernal(distance, spiky);
	});
	return (-this->_density) *  resultingPressureForce;
}

QCPVector2D FluidParticle::computeViscousForce(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingVisousForce(0, 0);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this || MathUtilities::isEqual(distance, 0))
			return;
		resultingVisousForce +=  (particle->_velocity - this->_velocity) * (particle->_mass / particle->_density)
			* this->applyKernal(distance, visc);
	});

	return this->_viscosity * resultingVisousForce;
}

//this method will compute the surface normal(which is the gradient to the color field)
QCPVector2D FluidParticle::computeSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingSurfaceNormal(0.0, 0.0);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this || MathUtilities::isEqual(distance, 0))
			return;
		auto vec = this->positionVector - particle->positionVector;
		double kernel = this->applyKernal(distance, gradPoly6);
		resultingSurfaceNormal += vec * (particle->_mass / particle->_density)*kernel;
	});
	return resultingSurfaceNormal;
}


//this method we compute the laplacian of the color field to use it to compute the surface tension
double FluidParticle::computeLaplacianSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	double resultingSurfaceNormal = 0.0;

	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		double kernel = this->applyKernal(distance, lapPoly6);
		resultingSurfaceNormal += (particle->_mass / particle->_density)*kernel;
	});

	return resultingSurfaceNormal;
}

QCPVector2D FluidParticle::computeSurfaceTension(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingSurfaceTension(0.0, 0.0);
	auto surfaceNormal = this->computeSurfaceNormal(surroundingBodies, radius);
	double magnitude = surfaceNormal.length();
	if (magnitude >= this->_surfaceThreshold)
	{
		surfaceNormal.normalize();
		resultingSurfaceTension = this->computeLaplacianSurfaceNormal(surroundingBodies, radius) * surfaceNormal;
	}

	return -1 * this->_tensionCoefcioant * resultingSurfaceTension;
}

void FluidParticle::runFunctionOverFluidParicles(const QVector<BodiesVector*>& surroundingBodies, double radius, const function <void(FluidParticle*, double)>&& func)
{
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			if (body->bodyType == fluid)
			{
				auto particle = static_cast<FluidParticle*>(body);
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				if (distance <= radius) {
					func(particle, distance);
				}
			}
		}
	}
}

QCPVector2D FluidParticle::computeSumOfForces(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D pressureForce = this->computePressureForce(surroundingBodies, radius);
	QCPVector2D viscousForce = this->computeViscousForce(surroundingBodies, radius);
	QCPVector2D gravityForce = this->engine->getGravity() * this->_restDensity;
	QCPVector2D surfaceTensionForce = this->computeSurfaceTension(surroundingBodies, radius);
	return pressureForce + viscousForce + gravityForce + surfaceTensionForce;
}

void FluidParticle::applyLeapFrogTimeStepIntegration()
{
	auto accelration = this->_force / this->_restDensity;
	if (!this->_isFirstIteration)
	{
		this->_leapFrogNextStep = this->_leapFrogPreviousStep + (engine->timeDelta * accelration);
	}
	else
	{
		this->_leapFrogNextStep = QCPVector2D(0, 0);
		this->_leapFrogPreviousStep = this->_velocity - (0.5 * engine->timeDelta * accelration);
		this->_isFirstIteration = false;
	}
	this->positionVector += this->_leapFrogNextStep * engine->timeDelta;
	this->setPosition(this->positionVector.toPointF());

}

void FluidParticle::detectCollision(const QRectF& boundingBox)
{
	if (boundingBox.contains(this->position))
	{
		return;
	}

	QPointF centerPoint = boundingBox.center();
	QCPVector2D extentVector = QCPVector2D(boundingBox.width(), boundingBox.height());

	auto rightSide = QCPVector2D(qMax(0.0, this->positionVector.x()), qMax(0.0, this->positionVector.y()));

	auto contactPoint = QCPVector2D(qMin((extentVector).x(), rightSide.x()), qMin((extentVector).y(), rightSide.y()));

	auto penterationDepth = this->positionVector.distanceToPoint(contactPoint);

	QCPVector2D normal = this->positionVector - contactPoint;
	normal.normalize();

	this->setPosition(contactPoint.toPointF());
	this->_leapFrogNextStep -= (1 + this->_restitution) * this->_leapFrogNextStep.dot(normal) * normal;
}

int FluidParticle::signumFunction(double x)
{
	if (MathUtilities::isEqual(x, 0.0))
		return 0;
	if (x < 0.0)
		return -1;
	return 1;
}

/*
important note about threading:
first density should computed for all particles
second pressure should be computed for all particles
then the rest can work simultaneously
*/
void FluidParticle::calculateInteractionWithBodies(const QVector<BodiesVector*>& surroundingBodies, int calculationOperation)
{
	// note that density and pressure needs to be computed here
	// because their values are important in the other methods
	auto engine = PhysicsEngine::shared();
	double radius = engine->getUnsafeBodiesGrid().squareSideInCentimeters();

	if (calculationOperation == 0) {
		this->_density = this->computeDensity(surroundingBodies, radius);
		//here gas constant and rest density should variables taken from user input
		this->_pressure = this->computePressure();
	}
	else if (calculationOperation == 1) {
		//here gravity should also be taken from user input
		this->_force = this->computeSumOfForces(surroundingBodies, radius);
	}
}

void FluidParticle::applyInteraction()
{
	this->applyLeapFrogTimeStepIntegration();
	auto size = this->engine->getUnsafeBodiesGrid().sizeInCentimeters();

	this->detectCollision(QRectF(0.0, 0.0, size.width(), size.height()));

	this->_velocity = (this->_leapFrogPreviousStep + this->_leapFrogNextStep) / 2.0;

	this->_leapFrogPreviousStep = this->_leapFrogNextStep;

}



