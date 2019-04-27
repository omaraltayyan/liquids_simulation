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
	const QSize& sizeInMeters() const;
	void sizeInMeters(const QSize& sizeInMeters);

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
	QRect transformFromMetersToSquares(QRect const & metersRect);
	int transformFromMetersToSquares(int const & meters);
	QPoint transformFromMetersToSquares(QPoint const & metersRect);
	QSize transformFromMetersToSquares(QSize const & metersRect);

	// the side of the square this grid gets
	// divided into, in meters
	int squareSideInMeters();
	void squareSideInMeters(int sizeInMeters);


	// returns the dimensions of the given body
	// inside the grid relative to the squares
	// coordinate system, if the body is out of bounds,
	// then we return a NULL QRect
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
	QVector<int> Grid::squareDimensionsSquareNumbers(QRect const & squareDimensions);
	QVector<int> Grid::metersDimensionsSquareNumbers(QRect const & metersDimensions);
	QVector<int> getBodySquareNumbers(Body const & body);
	QVector<int> getBodySquareNumbersWithSurroundingSquares(Body const & body);

	Grid();
	~Grid();

private:

	void resetSquareCoordinates();

	QSize _sizeInMeters;
	QSize _sizeInSquares;
	QRect _rectInSquares;
	int _squareSideInMeters = 0;
	int _numSquares = 0;

	QVector<BodiesVector*> squaresBodies;
	BodiesVector allBodies;
};

