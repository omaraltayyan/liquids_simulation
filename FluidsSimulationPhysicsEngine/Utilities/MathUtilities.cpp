#include "Utilities\MathUtilities.h"

bool MathUtilities::isEqual(double x, double y)
{
	const double epsilon = 1e-7;
	return std::abs(x - y) <= epsilon * std::abs(x);
}

MathUtilities::MathUtilities()
{
}


MathUtilities::~MathUtilities()
{
}
