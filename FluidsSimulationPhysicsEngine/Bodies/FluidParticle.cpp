#include "FluidParticle.h"



FluidParticle::FluidParticle(double dynsity, double viscosity, double pressure,double mass, float Xposition, float Yposition)
{
	_dynsity = dynsity;
	_viscosity = viscosity;
	_pressure = pressure;
	_mass = mass;
	_position = new QVector2D(Xposition, Yposition);
}


FluidParticle::~FluidParticle()
{
}
