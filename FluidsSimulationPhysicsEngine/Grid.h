#include "Bodies\Body.h"
#include <QRect>
#include <QVector2D>
#include <QVector>

#pragma once

typedef QVector<Body*> BodiesVector;

class Grid
{

public:
	// the grid window size
	const QSizeF& sizeInMeters() const;
	void sizeInMeters(const QSizeF& sizeInMeters);

	// number of squares this grid
	int numSquares();

	// gets the size of this grid in squares
	const QSize& sizeInSquares();

	// a rectangle representing the grid in squares
	const QRect& rectInSquares();

	/*
	* transforms a given coordinate set from meters to be in
	* squares
	*/
	QRectF transformFromMetersToSquares(QRectF const & metersRect);
	qreal transformFromMetersToSquares(qreal const & meters);
	QPointF transformFromMetersToSquares(QPointF const & metersRect);
	QSizeF transformFromMetersToSquares(QSizeF const & metersRect);

	// the side of the square this grid gets
	// divided into, in meters
	qreal squareSideInMeters();
	void squareSideInMeters(qreal sizeInMeters);


	// returns the dimensions of the given body
	// inside the grid relative to the squares
	// coordinate system, if the body is out of bounds,
	// then we return a NULL QRectF
	QRect getBodySquareDimensions(Body const & body);
	QRect getBodySquareDimentionsWithSurroundingSquares(Body const & body);

	/*
	*
	*/
	void addBodiesToGrid(const BodiesVector& bodies);

	void updateBodiesInGrid();

	/*
		takes in a Rectangle with the dimensions in squares,
		and returns an array of the single sequential
		index of each square these dimensions cover.
		WARNING: Make sure to delete the array after being done from it.
	*/
	QVector<int> Grid::squareDimensionsSquareIndexs(QRect const & squareDimensions);
	QVector<int> Grid::metersDimensionsSquareIndexs(QRectF const & metersDimensions);
	QVector<int> getBodySquareIndexs(Body const & body);
	QVector<int> getBodySquareIndexsWithSurroundingSquares(Body const & body);


	Grid(const QSizeF& sizeInMeters, qreal squareSideInMeters);
	~Grid();

private:

	void resetSquareCoordinates();

	QSizeF _sizeInMeters = QSizeF();
	QSize _sizeInSquares = QSize();
	QRect _rectInSquares = QRect();
	qreal _squareSideInMeters = 0;
	int _numSquares = 0;

	QVector<BodiesVector*> squaresBodies;
	BodiesVector allBodies;
};

