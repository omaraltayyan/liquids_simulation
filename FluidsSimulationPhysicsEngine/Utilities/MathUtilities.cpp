#include "Utilities\MathUtilities.h"

bool MathUtilities::isEqual(double x, double y)
{
	const double epsilon = 1e-8;
	return std::abs(x - y) <= epsilon;
}

QCPVector2D MathUtilities::abs(const QCPVector2D& vector) {
	QCPVector2D result = QCPVector2D(vector);
	result.setX(std::abs(vector.x()));
	result.setY(std::abs(vector.y()));

	return result;
}

double MathUtilities::max(const QCPVector2D& vector) {
	return std::max(vector.x(), vector.y());

}

double MathUtilities::min(const QCPVector2D& vector) {
	return std::min(vector.x(), vector.y());
}

QCPVector2D MathUtilities::randomVector(double until, double from) {
	float randomDistanceX = from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (until - from)));
	float randomDistanceY = from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (until - from)));

	return QCPVector2D(randomDistanceX, randomDistanceY);
}


MathUtilities::MathUtilities()
{
}


MathUtilities::~MathUtilities()
{
}
