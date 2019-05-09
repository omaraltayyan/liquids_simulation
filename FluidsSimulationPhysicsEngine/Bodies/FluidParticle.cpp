#include "FluidParticle.h"
#include <qmath.h>



FluidParticle::FluidParticle( double viscosity, double mass, float Xposition, float Yposition)
{	
	_viscosity = viscosity;
	_mass = mass;
	_position = new QVector2D(Xposition, Yposition);
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
			return (15 / (M_PI*qPow(radius, 6))) * qPow((radius - distance), 3);
		case visc:
			return (15 / (2 * M_PI*qPow(radius, 3))) *
				(-(qPow(distance, 3) / (2 * M_PI*qPow(radius, 3))) + 
				(qPow(distance, 2) / qPow(radius, 2)) + (radius / (2 * distance)) - 1);
		default:
			return 0.0;
		}		
	

}

double FluidParticle::computeDynsity(QVector<BodiesVector*>& neighboringBodies,double radius)
{
	return 0.0;
}

void FluidParticle::updateDynsity()
{
}
