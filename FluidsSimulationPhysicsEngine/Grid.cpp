#include "Grid.h"
#include "Utilities\Memory.h"
using namespace std;
#include <cmath>
#include "Utilities\MathUtilities.h"
#include <assert.h>     /* assert */

Grid::Grid(const QSizeF& sizeInCentimeters, qreal squareSideInCentimeters)
{
	this->bodiesSquareIndexs = QVector<QVector<int>>();
	this->sizeInCentimeters(sizeInCentimeters);
	this->setSquareSideInCentimeters(squareSideInCentimeters);
}
Grid::~Grid()
{
	if (squaresBodies.length() > 0) {
		for (int i = 0; i < squaresBodies.length(); i++)
		{
			delete squaresBodies[i];
		}
		squaresBodies.clear();
	}

}

BodiesVector Grid::getAllBodies() {
	return allBodies;
}

void Grid::addBodiesToGrid(const BodiesVector& bodies) {

	if (bodies.length() == 0)
	{
		return;
	}

	allBodies.append(bodies);

	auto squaresBodiesCounts = new int[this->numSquares()]();

	this->bodiesSquareIndexs.reserve(this->bodiesSquareIndexs.length() + bodies.length());
	
	for (int i = allBodies.length() - bodies.length(); i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		bodiesSquareIndexs.push_back(this->getBodySquareIndexs(*body));
		if (bodiesSquareIndexs.last().length() == 0) {
			bodiesSquareIndexs.removeLast();
			delete allBodies.at(i);
			allBodies.removeAt(i);
			i--;
			continue;
		}
		for each(int squareIndex in bodiesSquareIndexs.at(i)) {
				squaresBodiesCounts[squareIndex]++;
		}
	}

	if (squaresBodies.length() == 0) {
		squaresBodies = QVector<BodiesVector*>(this->numSquares());
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i] = new BodiesVector(squaresBodiesCounts[i]);
		}
	}
	else {
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i]->reserve(squaresBodies[i]->length() + squaresBodiesCounts[i]);
		}
	}

	for (int i = allBodies.length() - bodies.length(); i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		for each(int squareIndex in bodiesSquareIndexs[i]) {
			squaresBodies[squareIndex]->push_back(body);
		}
	}

	delete[] squaresBodiesCounts;
}

void Grid::updateBodiesInGrid() {

	auto squaresBodiesCounts = new int[this->numSquares()]();

	this->bodiesSquareIndexs = QVector<QVector<int>>(allBodies.length());

	for (int i = 0; i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		bodiesSquareIndexs[i] = this->getBodySquareIndexs(*body);
		if (bodiesSquareIndexs[i].length() == 0) {
			bodiesSquareIndexs.removeAt(i);
			delete allBodies.at(i);
			allBodies.removeAt(i);

			i--;
			continue;
		}
		for each(int squareIndex in bodiesSquareIndexs.at(i)) {
			squaresBodiesCounts[squareIndex]++;
		}
	}

	if (squaresBodies.length() == 0) {
		squaresBodies = QVector<BodiesVector*>(this->numSquares());
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i] = new BodiesVector(squaresBodiesCounts[i]);
		}
	}
	else {
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i]->clear();
			squaresBodies[i]->reserve(squaresBodiesCounts[i]);

		}
	}

	for (int i = 0; i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		for each(int squareIndex in bodiesSquareIndexs[i]) {
				squaresBodies[squareIndex]->push_back(body);
			
		}
	}

	for (int i = 0; i < this->numSquares(); i++)
	{
		squaresBodies[i]->squeeze();
	}

	delete[] squaresBodiesCounts;
}



int Grid::bodiesCount() {
	return allBodies.length();
}

Body* Grid::getBodyAtIndex(int index) {
	return allBodies.at(index);
}

QVector<BodiesVector*> Grid::getBodySourroundingBodiesVectors(int bodyIndex) {
	auto indexes = bodiesSquareIndexs[bodyIndex];
	auto squaresVector = QVector<BodiesVector*>();
	squaresVector.reserve(indexes.length());
	for each(int squareIndex in indexes) {
		squaresVector.push_back(squaresBodies[squareIndex]);
	}
	return squaresVector;
}


const QRect& Grid::rectInSquares() {
	if (!_rectInSquares.isValid()) {
		_rectInSquares = QRect(0, 0, this->sizeInSquares().width(), this->sizeInSquares().height());
	}

	return _rectInSquares;
}

const QSize& Grid::sizeInSquares() {
	if (!_sizeInSquares.isValid()) {
		// both grid dimensions must be a multiple
		// of the side of the squares we divide this
		// grid into
		assert(MathUtilities::isEqual(fmod(this->sizeInCentimeters().width(), this->squareSideInCentimeters()), 0));
		assert(MathUtilities::isEqual(fmod(this->sizeInCentimeters().height(), this->squareSideInCentimeters()), 0));

		_sizeInSquares = this->transformFromCentimetersToSquares(this->sizeInCentimeters()).toSize();
	}
	return _sizeInSquares;
}

int Grid::numSquares() {
	if (_numSquares <= 0) {
		_numSquares = this->sizeInSquares().height() * this->sizeInSquares().width();
	}
	return _numSquares;
}

// the grid window size
const QSizeF& Grid::sizeInCentimeters() const {
	return _sizeInCentimeters;
}

void Grid::sizeInCentimeters(const QSizeF& sizeInCentimeters) {
	_sizeInCentimeters = sizeInCentimeters;
	resetSquareCoordinates();
}

// the side of the square this grid gets
// divided into, in centimeters
qreal Grid::squareSideInCentimeters() {
	return _squareSideInCentimeters;
}

void Grid::setSquareSideInCentimeters(qreal squareSideInCentimeters) {
	_squareSideInCentimeters = squareSideInCentimeters;
	resetSquareCoordinates();
}

void Grid::resetSquareCoordinates() {
	_sizeInSquares = QSize();
	_numSquares = 0;
	_rectInSquares = QRect();
}

inline QRect Grid::getBodySquareDimensions(Body const & body) {
	return this->transformFromCentimetersToSquares(body.boundingRect).toAlignedRect();
}

QRect Grid::getBodySquareDimentionsWithSurroundingSquares(Body const & body) {
	auto squaresDimensions = getBodySquareDimensions(body);
	squaresDimensions += QMargins(1, 1, 1, 1);
	squaresDimensions &= this->rectInSquares();
	return squaresDimensions;
}

QVector<int> Grid::getBodySquareIndexsWithSurroundingSquares(Body const & body) {
	auto sourroundingRect = getBodySquareDimentionsWithSurroundingSquares(body);
	return squareDimensionsSquareIndexs(sourroundingRect);
}

inline QVector<int> Grid::getBodySquareIndexs(Body const & body) {
	return this->centimetersDimensionsSquareIndexs(body.boundingRect);
}

inline QVector<int> Grid::centimetersDimensionsSquareIndexs(QRectF const & centimetersDimensions) {
	auto squaresDimensions = this->transformFromCentimetersToSquares(centimetersDimensions).toAlignedRect();
	return this->squareDimensionsSquareIndexs(squaresDimensions);
}


inline QVector<int> Grid::squareDimensionsSquareIndexs(QRect const & squareDimensions) {
	int squaresInDimensions = squareDimensions.width() * squareDimensions.height();
	auto numberOfSquares = this->numSquares();

	auto indexesArray = QVector<int>();
	indexesArray.reserve(squaresInDimensions);
	auto topPoint = squareDimensions.topLeft();
	auto bottomPoint = squareDimensions.bottomRight();
	if (squaresInDimensions == 1) {
		int squareIndex = topPoint.x() + topPoint.y() * this->sizeInSquares().width();
		if (squareIndex >= 0 && squareIndex < numberOfSquares) {
			indexesArray.push_back(squareIndex);
		}
	}
	if (squaresInDimensions >= 2) {
		for (int i = topPoint.x(); i <= bottomPoint.x(); i++) {
			for (int j = topPoint.y(); j <= bottomPoint.y(); j++)
			{
				int squareIndex = i + j * this->sizeInSquares().width();
				if (squareIndex >= 0 && squareIndex < numberOfSquares) {
					indexesArray.push_back(squareIndex);
				}
			}
		}
	}
	return indexesArray;
}

inline QRectF Grid::transformFromCentimetersToSquares(QRectF const & centimetersRect) {
	return QRectF(transformFromCentimetersToSquares(centimetersRect.topLeft()),
		transformFromCentimetersToSquares(centimetersRect.bottomRight()));
}

inline QSizeF Grid::transformFromCentimetersToSquares(QSizeF const & size) {
	return QSizeF(this->transformFromCentimetersToSquares(size.width()),
		this->transformFromCentimetersToSquares(size.height()));
}

inline QPointF Grid::transformFromCentimetersToSquares(QPointF const & point) {
	auto p1 = this->transformFromCentimetersToSquares(point.x());
	auto p2 = this->transformFromCentimetersToSquares(point.y());
	auto p = QPointF(p1, p2);
	return p;
}

inline qreal Grid::transformFromCentimetersToSquares(qreal const & centimeters) {
	if (this->squareSideInCentimeters() == 0)
		return 0;
	return centimeters / this->squareSideInCentimeters();
}
