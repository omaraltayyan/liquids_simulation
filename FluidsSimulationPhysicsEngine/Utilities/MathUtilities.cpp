#include "Utilities\MathUtilities.h"

bool MathUtilities::isEqual(double x, double y)
{
	const double epsilon = 1e-8;
	return std::abs(x - y) <= epsilon;
}

MathUtilities::MathUtilities()
{
}


MathUtilities::~MathUtilities()
{
}
