#pragma once
#include <qcolor.h>

struct Material
{
	double emittedParticleViscosity;
	double emittedParticleMass;
	double emittedParticleSurfaceTension;
	double emittedParticleThreshold;
	double emittedParticleGasConstant;
	double emittedParticleRestDensity;
	double emittedParticleRestitution;
	double emittedParticleBuoyancy;
	QColor emittedParticleColor;

};

static Material water = Material{
	3.5,
	0.02,
	0.0728,
	7.065,
	3,
	998.29,
	0,
	0,
	QColor(173, 216, 230)
};

static Material mucus = Material{
	36,
	0.04,
	6,
	5,
	5,
	1000,
	0.5,
	0,
	QColor(255, 255, 0)
};

static Material steam = Material{
	0.01,
	5e-5,
	0,
	1e300,
	4,
	0.59,
	0,
	5,
	QColor(0, 150, 0)
};
