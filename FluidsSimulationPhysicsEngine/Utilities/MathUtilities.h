#pragma once
#include <cmath>
#include <QCPVector2D.h>

class MathUtilities
{
public:

	static bool isEqual(double x, double y);
	static QCPVector2D abs(const QCPVector2D& vector);
	static double max(const QCPVector2D& vector);
	static double min(const QCPVector2D& vector);

	static QCPVector2D randomVector(double until, double from = 0);

	MathUtilities();
	~MathUtilities();
};

