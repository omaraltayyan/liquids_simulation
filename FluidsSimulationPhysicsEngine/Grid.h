
#pragma once
#include "Bodies\Body.h"
#include <qmath.h>

struct KernelsInfo {
	double radius;
	double radius_2;
	double radius_4;

	double poly6Left;
	double gradAndLapPoly6Left;
	double spikyLeft;
	double viscLeft;

	/* not supported */
	KernelsInfo() { }

	KernelsInfo(double radius) {
		this->radius = radius;
		this->radius_2 = radius * radius;
		this->radius_4 = radius_2 * radius_2;

		this->poly6Left = 315 / (64 * M_PI*radius_4*radius_4*radius);

		this->gradAndLapPoly6Left = -945 / (32 * M_PI * radius_4*radius_4*radius);

		this->spikyLeft = -45 / (M_PI * radius_4*radius_2);

		this->viscLeft = 45 / (M_PI *radius_4*radius_2);
	}

};

class Grid
{

public:
	// the grid window size
	const QSizeF& sizeInCentimeters() const;
	void sizeInCentimeters(const QSizeF& sizeInCentimeters);

	// number of squares this grid
	int numSquares();

	// gets the size of this grid in squares
	const QSize& sizeInSquares();

	// a rectangle representing the grid in squares
	const QRect& rectInSquares();

	/*
	* transforms a given coordinate set from centimeters to be in
	* squares
	*/
	QRectF transformFromCentimetersToSquares(QRectF const & centimetersRect);
	qreal transformFromCentimetersToSquares(qreal const & centimeters);
	QPointF transformFromCentimetersToSquares(QPointF const & centimetersRect);
	QSizeF transformFromCentimetersToSquares(QSizeF const & centimetersRect);

	// the side of the square this grid gets
	// divided into, in centimeters
	qreal squareSideInCentimeters();
	void setSquareSideInCentimeters(qreal sizeInCentimeters);

	const KernelsInfo& getKernelsInfo();


	// get bodies in grid info
	BodiesVector getAllBodies();
	int bodiesCount();
	Body* getBodyAtIndex(int index);
	QVector<BodiesVector*> getBodySourroundingBodiesVectors(int bodyIndex);

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
	void clearBodies();
	void updateBodiesInGrid();

	/*
	takes in a Rectangle with the dimensions in squares,
	and returns an array of the single sequential
	index of each square these dimensions cover.
	WARNING: Make sure to delete the array after being done from it.
	*/
	QVector<int> Grid::squareDimensionsSquareIndexs(QRect const & squareDimensions);
	QVector<int> Grid::centimetersDimensionsSquareIndexs(QRectF const & centimetersDimensions);
	QVector<int> getBodySquareIndexs(Body const & body);
	QVector<int> getBodySquareIndexsWithSurroundingSquares(Body const & body);


	Grid(const QSizeF& sizeInCentimeters, qreal squareSideInCentimeters);
	~Grid();

private:

	QVector<QVector<int>> bodiesSquareIndexs;

	void resetSquareCoordinates();
	
	KernelsInfo kernelsInfo;
	
	QSizeF _sizeInCentimeters = QSizeF();
	QSize _sizeInSquares = QSize();
	QRect _rectInSquares = QRect();
	qreal _squareSideInCentimeters = 0;
	int _numSquares = 0;

	QVector<BodiesVector*> squaresBodies;
	BodiesVector allBodies;
};

