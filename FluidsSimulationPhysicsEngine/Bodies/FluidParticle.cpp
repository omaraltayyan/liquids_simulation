#include "FluidParticle.h"
#include <qmath.h>



FluidParticle::FluidParticle()
{
}

FluidParticle::FluidParticle( double viscosity, double mass, float Xposition, float Yposition)
{	
	_viscosity = viscosity;
	_mass = mass;
	_position = new QVector2D(Xposition, Yposition);
	_velocity = new QVector2D(0.0f, 0.0f);
}


FluidParticle::~FluidParticle()
{
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
				double distance = this->_position->distanceToPoint(*particle->_position);
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

double FluidParticle::computePressureForce(QVector<BodiesVector*> surroundingBodies, double radius)
{
	double resultingPressureForce = 0.0;
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{
				double distance = this->_position->distanceToPoint(*particle->_position);
				
				if (distance < radius)
				{
					resultingPressureForce += particle->_mass * ((this->_pressure+particle->_pressure) / 2 * particle->_dynsity)
						* this->applyKernal(distance, radius, spiky);
				}

			}
		}
	}
	return -1 * resultingPressureForce;
}

double FluidParticle::computeViscousForce(QVector<BodiesVector*> surroundingBodies, double radius)
{
	double resultingVisousForce = 0.0;
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (particle != NULL)
			{
				double distance = this->_position->distanceToPoint(*particle->_position);
				if (distance < radius)
				{						
					resultingVisousForce += particle->_mass * ((particle->_velocity - this->_velocity) / particle->_dynsity)
						* this->applyKernal(distance, radius, visc);
				}

			}
		}
	}
	
	return this->_viscosity * resultingVisousForce;
}

double FluidParticle::computeSumOfForces(QVector<BodiesVector*> surroundingBodies, double radius)
{
	return 0.0;
}



