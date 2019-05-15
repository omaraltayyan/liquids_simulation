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
}


double FluidParticle::applyKernal(double distance, double radius, SmoothingKernals kernal)
{
	
		switch (kernal)
		{
		case poly6:
			return (315 / (64*M_PI*qPow(radius,9))) * qPow(((radius*radius) - (distance*distance)),3);			
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
				if (distance < radius)
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
	return gasConstant * (density - restDynsity);
}

QVector2D FluidParticle::computePressureForce(QVector<BodiesVector*> surroundingBodies, double radius)
{
	QVector2D resultingPressureForce(0.0,0.0);
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{
				auto vec = particle->positionVector - this->positionVector;
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				
				if (distance < radius)
				{
					resultingPressureForce += -vec.normalized() * particle->_mass * ((this->_pressure+particle->_pressure) / 2 * particle->_density)
						* this->applyKernal(distance, radius, spiky);
				}

			}
		}
	}
	return  resultingPressureForce;
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
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{				
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				if (distance < radius)
				{
					resultingVisousForce += particle->_mass * (((particle->_velocity) - (this->_velocity)) / particle->_density)
						* this->applyKernal(distance, radius, visc);
				}

			}
		}
	}
	
	return this->_viscosity * resultingVisousForce;
}

QVector2D FluidParticle::computeSumOfForces(QVector<BodiesVector*> surroundingBodies, double radius)
{	
	QVector2D pressureForce = this->computePressureForce(surroundingBodies, radius);
	QVector2D viscousForce = this->computeViscousForce(surroundingBodies, radius);
	QVector2D gravityForce = this->engine->gravity * this->_density;
	return pressureForce + viscousForce + gravityForce;
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
}



