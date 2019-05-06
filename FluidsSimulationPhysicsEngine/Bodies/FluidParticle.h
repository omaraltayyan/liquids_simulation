#pragma once

#include "Particle.h"
#include <QVector2D>

class FluidParticle: public Particle
{
public:
	FluidParticle(double dynsity,double viscosity,double pressure,double mass,float Xposition,float Yposition);
	~FluidParticle();
private:
	double _dynsity;
	double _viscosity;
	double _pressure;
	double _mass;
	QVector2D* _velocity;
	QVector2D* _position;
	QVector2D* _accelration;
	
	 
};

