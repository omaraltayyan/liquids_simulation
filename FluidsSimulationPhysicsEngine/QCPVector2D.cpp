#include "QCPVector2D.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPVector2D
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPVector2D
\brief Represents two doubles as a mathematical 2D vector

This class acts as a replacement for QVector2D with the advantage of double precision instead of
single, and some convenience methods tailored for the QCustomPlot library.
*/

/* start documentation of inline functions */

/*! \fn void QCPVector2D::setX(double x)

Sets the x coordinate of this vector to \a x.

\see setY
*/

/*! \fn void QCPVector2D::setY(double y)

Sets the y coordinate of this vector to \a y.

\see setX
*/

/*! \fn double QCPVector2D::length() const

Returns the length of this vector.

\see lengthSquared
*/

/*! \fn double QCPVector2D::lengthSquared() const

Returns the squared length of this vector. In some situations, e.g. when just trying to find the
shortest vector of a group, this is faster than calculating \ref length, because it avoids
calculation of a square root.

\see length
*/

/*! \fn QPoint QCPVector2D::toPoint() const

Returns a QPoint which has the x and y coordinates of this vector, truncating any floating point
information.

\see toPointF
*/

/*! \fn QPointF QCPVector2D::toPointF() const

Returns a QPointF which has the x and y coordinates of this vector.

\see toPoint
*/

/*! \fn bool QCPVector2D::isNull() const

Returns whether this vector is null. A vector is null if \c qIsNull returns true for both x and y
coordinates, i.e. if both are binary equal to 0.
*/

/*! \fn QCPVector2D QCPVector2D::perpendicular() const

Returns a vector perpendicular to this vector, with the same length.
*/

/*! \fn double QCPVector2D::dot() const

Returns the dot/scalar product of this vector with the specified vector \a vec.
*/

/* end documentation of inline functions */

/*!
Creates a QCPVector2D object and initializes the x and y coordinates to 0.
*/
QCPVector2D::QCPVector2D() :
	mX(0),
	mY(0)
{
}

/*!
Creates a QCPVector2D object and initializes the \a x and \a y coordinates with the specified
values.
*/
QCPVector2D::QCPVector2D(double x, double y) :
	mX(x),
	mY(y)
{
}

/*!
Creates a QCPVector2D object and initializes the x and y coordinates respective coordinates of
the specified \a point.
*/
QCPVector2D::QCPVector2D(const QPoint &point) :
	mX(point.x()),
	mY(point.y())
{
}

/*!
Creates a QCPVector2D object and initializes the x and y coordinates respective coordinates of
the specified \a point.
*/
QCPVector2D::QCPVector2D(const QPointF &point) :
	mX(point.x()),
	mY(point.y())
{
}

/*!
Normalizes this vector. After this operation, the length of the vector is equal to 1.

\see normalized, length, lengthSquared
*/
void QCPVector2D::normalize()
{
	double len = length();
	mX /= len;
	mY /= len;
}

/*!
Returns a normalized version of this vector. The length of the returned vector is equal to 1.

\see normalize, length, lengthSquared
*/
QCPVector2D QCPVector2D::normalized() const
{
	QCPVector2D result(mX, mY);
	result.normalize();
	return result;
}

/*! \overload

Returns the squared shortest distance of this vector (interpreted as a point) to the finite line
segment given by \a start and \a end.

\see distanceToStraightLine
*/
double QCPVector2D::distanceSquaredToLine(const QCPVector2D &start, const QCPVector2D &end) const
{
	QCPVector2D v(end - start);
	double vLengthSqr = v.lengthSquared();
	if (!qFuzzyIsNull(vLengthSqr))
	{
		double mu = v.dot(*this - start) / vLengthSqr;
		if (mu < 0)
			return (*this - start).lengthSquared();
		else if (mu > 1)
			return (*this - end).lengthSquared();
		else
			return ((start + mu*v) - *this).lengthSquared();
	}
	else
		return (*this - start).lengthSquared();
}

/*! \overload

Returns the squared shortest distance of this vector (interpreted as a point) to the finite line
segment given by \a line.

\see distanceToStraightLine
*/
double QCPVector2D::distanceSquaredToLine(const QLineF &line) const
{
	return distanceSquaredToLine(QCPVector2D(line.p1()), QCPVector2D(line.p2()));
}

double QCPVector2D::distanceToPoint(const QCPVector2D& point) const
{
	return (*this - point).length();
}

/*!
Returns the shortest distance of this vector (interpreted as a point) to the infinite straight
line given by a \a base point and a \a direction vector.

\see distanceSquaredToLine
*/
double QCPVector2D::distanceToStraightLine(const QCPVector2D &base, const QCPVector2D &direction) const
{
	return qAbs((*this - base).dot(direction.perpendicular())) / direction.length();
}

/*!
Scales this vector by the given \a factor, i.e. the x and y components are multiplied by \a
factor.
*/
QCPVector2D &QCPVector2D::operator*=(double factor)
{
	mX *= factor;
	mY *= factor;
	return *this;
}

/*!
Scales this vector by the given \a divisor, i.e. the x and y components are divided by \a
divisor.
*/
QCPVector2D &QCPVector2D::operator/=(double divisor)
{
	mX /= divisor;
	mY /= divisor;
	return *this;
}

/*!
Adds the given \a vector to this vector component-wise.
*/
QCPVector2D &QCPVector2D::operator+=(const QCPVector2D &vector)
{
	mX += vector.mX;
	mY += vector.mY;
	return *this;
}

/*!
subtracts the given \a vector from this vector component-wise.
*/
QCPVector2D &QCPVector2D::operator-=(const QCPVector2D &vector)
{
	mX -= vector.mX;
	mY -= vector.mY;
	return *this;
}