#pragma once
#include <qcolor.h>

struct Material
{
	double viscosity;
	double mass;
	double surfaceTension;
	double threshold;
	double gasConstant;
	double restDensity;
	double restitution;
	double buoyancy;
	QColor color;
	double radius;

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
	QColor(173, 216, 230),
	0.0457
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
	QColor(255, 255, 0),
	0.0726
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
	QColor(0, 150, 0),
	0.0624
};
