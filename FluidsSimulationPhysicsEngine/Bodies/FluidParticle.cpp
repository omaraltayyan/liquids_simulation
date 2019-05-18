#include "FluidParticle.h"
#include "PhysicsEngine.h"
#include <qmath.h>

FluidParticle::FluidParticle(const QPointF& position, PhysicsEngine* engine, qreal sizeRadius,
	double viscosity, double mass, double gasConstant, double restDesity,
	double surfaceTension, double threshold) : Particle(position, engine, sizeRadius)
{
	this->radius_2 = -100.0;
	this->radius_4 = -100.0;
	_viscosity = viscosity;
	_mass = mass;
	_gasConstant = gasConstant;
	_restDensity = restDesity;
	_velocity.setX(0.0);
	_velocity.setY(0.0);

	_accelration.setX(0.0);
	_accelration.setY(0.0);

	_velocityHalfStep.setX(0);
	_velocityHalfStep.setY(0);

	_tensionCoefcioant = surfaceTension;
	_surfaceThreshold = threshold;
	_isFirstIteration = true;
	this->bodyType = fluid;
}


double FluidParticle::applyKernal(double distance, double radius, SmoothingKernals kernal)
{
	if (radius_2 < 0.0)
	{
		radius_2 = radius * radius;
		radius_4 = radius_2 * radius_2;
	}

	double holder = radius_2 - (distance * distance);
	switch (kernal)
	{
	case poly6:
		return (315 / (64 * M_PI*radius_4*radius_4*radius)) * (holder*holder*holder);
	case gradPoly6:
		return -1 * (945 / (32 * M_PI * radius_4*radius_4*radius)) * (holder*holder);
	case lapPoly6:
		return ((-1 * 945) / (32 * M_PI * radius_4*radius_4*radius)) * holder * ((3 * radius*radius) - (7 * distance*distance));
	case spiky:
		return (-45 / (radius_4*radius_2)) * ((radius - distance) * (radius - distance));
	case visc:
		return (45 / (radius_4*radius_2)) * (radius - distance);
	default:
		return 0.0;
	}


}

double FluidParticle::computeDensity(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	double resultingDynsity = 0.0;
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		resultingDynsity += particle->_mass * this->applyKernal(distance, radius, poly6);
	});
	return resultingDynsity;
}

double FluidParticle::computePressure(double gasConstant, double restDynsity, double density)
{
	return gasConstant  * (density - restDynsity);
}

QVector2D FluidParticle::computePressureForce(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QVector2D resultingPressureForce(0.0, 0.0);
	auto relativePressureTerm = this->_pressure / (this->_density * this->_density);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this)
			return;
		auto vec = this->positionVector - particle->positionVector;
		vec.normalize();
		auto leftTerm = (relativePressureTerm + (particle->_pressure / (particle->_density * particle->_density))) * particle->_mass;
		resultingPressureForce += vec * leftTerm * this->applyKernal(distance, radius, spiky);
	});
	return (-this->_density) *  resultingPressureForce;
}

QVector2D FluidParticle::computeViscousForce(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QVector2D resultingVisousForce(0, 0);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this)
			return;
		resultingVisousForce += particle->_mass * (((particle->_velocity) - (this->_velocity)) / particle->_density)
			* this->applyKernal(distance, radius, visc);
	});

	return this->_viscosity * resultingVisousForce;
}

//this method will compute the surface normal(which is the gradient to the color field)
QVector2D FluidParticle::computeSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QVector2D resultingSurfaceNormal(0.0, 0.0);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this)
			return;
		auto vec = this->positionVector - particle->positionVector;
		double kernel = this->applyKernal(distance, radius, gradPoly6);
		resultingSurfaceNormal += vec * (particle->_mass / particle->_density)*kernel;
	});
	return resultingSurfaceNormal;
}


//this method we compute the laplacian of the color field to use it to compute the surface tension
double FluidParticle::computeLaplacianSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	double resultingSurfaceNormal = 0.0;

	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this)
			return;
		auto vec = this->positionVector - particle->positionVector;
		double kernel = this->applyKernal(distance, radius, lapPoly6);
		resultingSurfaceNormal += (particle->_mass / particle->_density)*kernel;
	});

	return resultingSurfaceNormal;
}

QVector2D FluidParticle::computeSurfaceTension(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QVector2D resultingSurfaceTension(0.0, 0.0);
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

QVector2D FluidParticle::computeSumOfForces(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QVector2D pressureForce = this->computePressureForce(surroundingBodies, radius);
	QVector2D viscousForce = this->computeViscousForce(surroundingBodies, radius);
	QVector2D gravityForce = this->engine->getGravity() * this->_density;
	QVector2D surfaceTensionForce = this->computeSurfaceTension(surroundingBodies, radius);
	return pressureForce + viscousForce + gravityForce + surfaceTensionForce;
}

void FluidParticle::applyLeapFrogTimeStepIntegration()
{
	auto accelration = this->_force / this->_density;
	if (!this->_isFirstIteration)
	{

		this->_velocityHalfStep += engine->timeDelta * accelration;
		this->_velocity = this->_velocityHalfStep + (accelration * (engine->timeDelta / 2));

	}
	else
	{
		this->_velocityHalfStep = this->_velocity + (accelration * (engine->timeDelta / 2));
		this->_velocity += accelration * engine->timeDelta;
		this->_isFirstIteration = false;
	}
	this->positionVector += this->_velocityHalfStep * engine->timeDelta;
	this->setPosition(this->positionVector.toPointF());

}

void FluidParticle::detectCollision(const QRectF& boundingBox)
{
	if (boundingBox.contains(this->position))
	{
		return;
	}

	// this code is correct
	QPointF centerPoint = boundingBox.center();
	QVector2D extentVector = QVector2D(boundingBox.width(), boundingBox.height());

	auto rightSide = QVector2D(qMax(0.0f, this->positionVector.x()), qMax(0.0f, this->positionVector.y()));

	auto contactPoint = QVector2D(qMin((extentVector).x(), rightSide.x()), qMin((extentVector).y(), rightSide.y()));

	if (contactPoint.x() < 0 || contactPoint.y() < 0) {
		printf("test");
	}
	auto penterationDepth = this->positionVector.distanceToPoint(contactPoint);

	QVector2D normal = this->positionVector - contactPoint;
	normal.normalize();

	this->setPosition(contactPoint.toPointF());
	double restitutionCoeficcciant = 0.35;
	this->_velocity -= (1 + restitutionCoeficcciant*(penterationDepth / (engine->timeDelta*this->_velocity.length())))
		* (this->_velocity.dotProduct(this->_velocity, normal))*normal;

	this->_velocityHalfStep -= (1 + restitutionCoeficcciant*(penterationDepth / (engine->timeDelta*this->_velocityHalfStep.length())))
		* (this->_velocityHalfStep.dotProduct(this->_velocityHalfStep, normal))*normal;


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
		this->_pressure = this->computePressure(this->_gasConstant, this->_restDensity, this->_density);
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

	/*
	this->detectCollision(0.0, size.width(), 0.0, 0.0);
	this->detectCollision(0.0, size.width(), size.height(), size.height());
	this->detectCollision(0.0, 0.0, 0.0, size.height());
	this->detectCollision(size.width(),size.width(), 0.0, size.height());*/
	/*double damp = 0.25;

	if (this->position.x() < 0)
	{
	this->positionVector -= this->_velocity * (1 - damp) * (this->position.x() / this->_velocity.x());
	this->positionVector.setX(-this->positionVector.x());

	this->_velocity.setX(-this->_velocity.x());
	this->_velocity *= damp;

	this->_velocityHalfStep.setX(-this->_velocityHalfStep.x());
	this->_velocityHalfStep *= damp;

	this->setPosition(this->positionVector.toPointF());


	}

	if (this->position.x() > size.width())
	{
	this->positionVector -= this->_velocity * (1 - damp) * ((this->position.x() - size.width()) / this->_velocity.x());
	this->positionVector.setX((2 * size.width()) - this->positionVector.x());

	this->_velocity.setX(-this->_velocity.x());
	this->_velocity *= damp;

	this->_velocityHalfStep.setX(-this->_velocityHalfStep.x());
	this->_velocityHalfStep *= damp;

	this->setPosition(this->positionVector.toPointF());
	}
	if (this->position.y() < 0)
	{
	this->positionVector -= this->_velocity * (1 - damp) * ((this->position.y()) / this->_velocity.y());
	this->positionVector.setY(-this->positionVector.y());

	this->_velocity.setY(-this->_velocity.y());
	this->_velocity *= damp;

	this->_velocityHalfStep.setY(-this->_velocityHalfStep.y());
	this->_velocityHalfStep *= damp;

	this->setPosition(this->positionVector.toPointF());
	}
	if (this->position.y() > size.height())
	{
	this->positionVector -= this->_velocity * (1 - damp) * ((this->position.y() - size.height()) / this->_velocity.y());
	this->positionVector.setY((2 * size.height()) - this->positionVector.y());

	this->_velocity.setY(-this->_velocity.y());
	this->_velocity *= damp;

	this->_velocityHalfStep.setY(-this->_velocityHalfStep.y());
	this->_velocityHalfStep *= damp;

	this->setPosition(this->positionVector.toPointF());
	}	*/

}



