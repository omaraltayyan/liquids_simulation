#include "FluidParticle.h"
#include <qmath.h>

FluidParticle::FluidParticle(const QPointF& position, qreal sizeRadius, double viscosity, double mass) : Particle(position, sizeRadius)
{	
	_viscosity = viscosity;
	_mass = mass;
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

double FluidParticle::computePressure(double gasConstant, double restDynsity, double dynsity)
{
	return gasConstant * (dynsity - restDynsity);
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
					resultingPressureForce += vec.normalized() * particle->_mass * ((this->_pressure+particle->_pressure) / 2 * particle->_dynsity)
						* this->applyKernal(distance, radius, spiky);
				}

			}
		}
	}
	return -1 * resultingPressureForce;
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
					resultingVisousForce += particle->_mass * (((particle->_velocity) - (this->_velocity)) / particle->_dynsity)
						* this->applyKernal(distance, radius, visc);
				}

			}
		}
	}
	
	return this->_viscosity * resultingVisousForce;
}

QVector2D FluidParticle::computeSumOfForces(QVector<BodiesVector*> surroundingBodies, double radius,double gravityValue)
{	
	QVector2D pressureForce = this->computePressureForce(surroundingBodies, radius);
	QVector2D viscousForce = this->computeViscousForce(surroundingBodies, radius);
	QVector2D gravity(0.0, gravityValue);
	QVector2D gravityForce = gravity * this->_dynsity;

	return pressureForce + viscousForce + gravityForce;
}

QVector2D FluidParticle::computeVelocityChange(double deltaTime, QVector2D sumForces)
{

	return this->_velocity + (deltaTime*sumForces/this->_dynsity);
}
/*
important note about threading:
	first dynsity should computed for all particles
	second pressure should be computed for all particles
	then the rest can work simultinously
*/
void FluidParticle::calculateInteractionWithBodies(QVector<BodiesVector*> surroundingBodies, int calculationOperation)
{
	// note that dynsity and pressure needs to be computed here
	// because their values are important in the other methods
	auto engine = PhysicsEngine::shared();
	double radius = engine->getUnsafeBodiesGrid().squareSideInMeters();
	this->_dynsity = this->computeDynsity(surroundingBodies,radius );

	//here gas constant and rest dynsity should variables taken from user input
	this->_pressure = this->computePressure(2000, 1000, this->_dynsity);

	//here gravity should also be taken from user input
	double gravity = 12000 * -9.8;
	this->_force = this->computeSumOfForces(surroundingBodies, radius, gravity);
	this->_velocity = this->computeVelocityChange(engine->timeDelta,this->_force);
}

void FluidParticle::applyInteraction()
{
	auto engine = PhysicsEngine::shared();
	this->_position = engine->timeDelta * this->_velocity;

}



