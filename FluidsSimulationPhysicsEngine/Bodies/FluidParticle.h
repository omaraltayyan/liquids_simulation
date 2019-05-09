#pragma once

#include "Particle.h"
#include <QVector2D>
#include <qmath.h>
#include "Grid.h"


enum SmoothingKernals
{
	poly6,
	spiky,
	visc
};
class FluidParticle: public Particle
{
public:
	FluidParticle(double viscosity,double mass,float Xposition,float Yposition);
	~FluidParticle();
	void updateDynsity();
private:
	double _dynsity;
	double _viscosity;
	double _pressure;
	double _mass;
	QVector2D* _velocity;
	QVector2D* _position;
	QVector2D* _accelration;
	double applyKernal(double distance, double radius, SmoothingKernals kernal);
	double computeDynsity(QVector<BodiesVector*>& neighboringBodies,double radius);
	
	 
};

