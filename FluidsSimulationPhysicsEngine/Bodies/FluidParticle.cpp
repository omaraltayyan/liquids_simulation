#include "FluidParticle.h"
#include "PhysicsEngine.h"
#include <qmath.h>

FluidParticle::FluidParticle(const QPointF& position, PhysicsEngine* engine, qreal sizeRadius,
	double viscosity, double mass, double gasConstant, double restDesity) : Particle(position, engine, sizeRadius)
{	
	_viscosity = viscosity;
	_mass = mass;
	_gasConstant = gasConstant;
	_restDensity = restDesity;
	_velocity.setX(0.0); 
	_velocity.setY(0.0);
	_tensionCoefcioant = 0.728;
	_surfaceThreshold = 0.007;
}


double FluidParticle::applyKernal(double distance, double radius, SmoothingKernals kernal)
{
	
	double holder = (radius * radius) - (distance * distance);
	switch (kernal)
	{
	case poly6:
		return (315 / (64 * M_PI*qPow(radius, 9))) * (holder*holder*holder);
	case gradPoly6:
		return -1 * (945 / (32 * M_PI * qPow(radius, 9))) * (holder*holder);
	case lapPoly6:
		return ((-1 * 945) / (32 * M_PI * qPow(radius, 9))) * holder * ((3*radius*radius) - (7*distance*distance));
	case spiky:
		return (-45 / (M_PI*qPow(radius, 6))) * qPow((radius - distance), 2);
	case visc:
		return (45 / (M_PI*qPow(radius, 6))) * (radius - distance);
	default:
		return 0.0;
	}
	

}

double FluidParticle::computeDynsity(QVector<BodiesVector*> surroundingBodies,double radius)
{
	double resultingDynsity = 0.0;
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{				
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				if (distance <= radius)
				{
					resultingDynsity += particle->_mass * this->applyKernal(distance, radius, poly6);
				}
				
			}
		}
	}
	return resultingDynsity;
}

double FluidParticle::computePressure(double gasConstant, double restDynsity, double density)
{
	//if (density < restDynsity) {
	//	return 0.01 * gasConstant * (density - restDynsity);
	//}
	return gasConstant * engine->timeDelta * (density - restDynsity);
}

QVector2D FluidParticle::computePressureForce(QVector<BodiesVector*> surroundingBodies, double radius)
{
	QVector2D resultingPressureForce(0.0,0.0);
	auto relativePressureTerm = this->_pressure / (this->_density * this->_density);
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			if (body == this)
				continue;
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{
				auto vec = this->positionVector - particle->positionVector;
				vec.normalize();
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				
				if (distance <= radius)
				{
					auto leftTerm = (relativePressureTerm + (particle->_pressure / (particle->_density * particle->_density))) * particle->_mass;
					resultingPressureForce += vec * leftTerm * this->applyKernal(distance, radius, spiky);
				}

			}
		}
	}
	return (-this->_density) *  resultingPressureForce;
}

QVector2D FluidParticle::computeViscousForce(QVector<BodiesVector*> surroundingBodies, double radius)
{
	QVector2D resultingVisousForce(0,0);
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			if (body == this)
				continue;
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{						
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				if (distance <= radius)
				{
					resultingVisousForce +=  particle->_mass * (((particle->_velocity) - (this->_velocity)) / particle->_density)
						* this->applyKernal(distance, radius, visc);
				}

			}
		}
	}
	
	return this->_viscosity * resultingVisousForce;
}

//important note : 
//when useLaplacian equlas false this method will compute the surface normal (whiche is the gradient to the color feild)
//when useLaplaican equals true this method wii compute the laplacian of the color feild to use it to compute the surfacetension
QVector2D FluidParticle::computeSurfaceNormal(QVector<BodiesVector*> surroundingBodies, double radius, bool useLaplacian)
{
	QVector2D resultingSurfaceNormal(0.0, 0.0);
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			if (body == this)
				continue;
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{
				auto vec = this->positionVector - particle->positionVector;
				vec.normalize();
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				if (distance <= radius)
				{
					double kernel = 0.0;
					if (useLaplacian)
						kernel = this->applyKernal(distance, radius, lapPoly6);
					else
						kernel = this->applyKernal(distance, radius, gradPoly6);					
					resultingSurfaceNormal += vec * (particle->_mass*particle->_density)*kernel;
				}
			}

		}
	}
	return resultingSurfaceNormal;
}

QVector2D FluidParticle::computeSurfaceTension(QVector<BodiesVector*> surroundingBodies, double radius)
{
	QVector2D resultingSurfaceTension(0.0, 0.0);
	auto surfaceNormal = this->computeSurfaceNormal(surroundingBodies, radius, false);
	double magnitude =qSqrt(surfaceNormal.x()*surfaceNormal.x() + surfaceNormal.y()*surfaceNormal.y());
	if ( magnitude > this->_surfaceThreshold)
	{
		resultingSurfaceTension = this->computeSurfaceNormal(surroundingBodies, radius, true) * (surfaceNormal/magnitude);
	}
	

	return -1 * this->_tensionCoefcioant * resultingSurfaceTension;
}

QVector2D FluidParticle::computeSumOfForces(QVector<BodiesVector*> surroundingBodies, double radius)
{	
	QVector2D pressureForce = this->computePressureForce(surroundingBodies, radius);
	QVector2D viscousForce = this->computeViscousForce(surroundingBodies, radius);
	QVector2D gravityForce = 0.1 * this->engine->gravity * this->_density;
	//QVector2D surfaceTensionForce = this->computeSurfaceTension(surroundingBodies,radius);
	return pressureForce + viscousForce + gravityForce;// +surfaceTensionForce;
}

QVector2D FluidParticle::computeVelocityChange(double deltaTime, QVector2D sumForces)
{
	return this->_velocity + (deltaTime*sumForces/this->_density);
}
/*
important note about threading:
	first density should computed for all particles
	second pressure should be computed for all particles
	then the rest can work simultaneously
*/
void FluidParticle::calculateInteractionWithBodies(QVector<BodiesVector*> surroundingBodies, int calculationOperation)
{
	// note that density and pressure needs to be computed here
	// because their values are important in the other methods
	auto engine = PhysicsEngine::shared();
	double radius = engine->getUnsafeBodiesGrid().squareSideInCentimeters();
	if (calculationOperation == 0) {
		this->_density = this->computeDynsity(surroundingBodies, radius);
		//here gas constant and rest density should variables taken from user input
		this->_pressure = this->computePressure(this->_gasConstant, this->_restDensity, this->_density);
	}
	else if (calculationOperation == 1) {
		//here gravity should also be taken from user input
		this->_force = this->computeSumOfForces(surroundingBodies, radius);
		this->_velocity = this->computeVelocityChange(engine->timeDelta, this->_force);
	}
}

void FluidParticle::applyInteraction()
{
	this->positionVector += (this->engine->timeDelta * this->_velocity);
	this->setPosition(this->positionVector.toPointF());
	auto size = this->engine->getUnsafeBodiesGrid().sizeInCentimeters();
	double damp = 0.25;

	if (this->position.x() < 0)
	{
		this->positionVector -= this->_velocity * (1 - damp) * (this->position.x()/this->_velocity.x());
		this->positionVector.setX(-this->positionVector.x());

		this->_velocity.setX(-this->_velocity.x());
		this->_velocity *= damp;

		this->setPosition(this->positionVector.toPointF());
	}
	
	if (this->position.x() > size.width())
	{
		this->positionVector -= this->_velocity * (1 - damp) * ((this->position.x()  - size.width())/ this->_velocity.x());
		this->positionVector.setX((2*size.width())-this->positionVector.x());

		this->_velocity.setX(-this->_velocity.x());
		this->_velocity *= damp;

		this->setPosition(this->positionVector.toPointF());
	}
	if (this->position.y() < 0)
	{	
		this->positionVector -= this->_velocity * (1 - damp) * ((this->position.y()) / this->_velocity.y());
		this->positionVector.setY( -this->positionVector.y());

		this->_velocity.setY(-this->_velocity.y());
		this->_velocity *= damp;

		this->setPosition(this->positionVector.toPointF());
	}
	if (this->position.y() > size.height())
	{
		this->positionVector -= this->_velocity * (1 - damp) * ((this->position.y() - size.height()) / this->_velocity.y());
		this->positionVector.setY((2*size.height())-this->positionVector.y());

		this->_velocity.setY(-this->_velocity.y());
		this->_velocity *= damp;

		this->setPosition(this->positionVector.toPointF());
	}
}



