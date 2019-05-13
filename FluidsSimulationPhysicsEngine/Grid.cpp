#include "Grid.h"
#include "Utilities\Memory.h"
using namespace std;
#include <cmath>
#include "Utilities\MathUtilities.h"
#include <assert.h>     /* assert */

Grid::Grid(const QSizeF& sizeInMeters, qreal squareSideInMeters)
{
	this->bodiesSquareIndexs = QVector<QVector<int>>();
	this->sizeInMeters(sizeInMeters);
	this->setSquareSideInMeters(squareSideInMeters);
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
		bodiesSquareIndexs[i] = this->getBodySquareIndexs(*body);
		for each(int squareIndex in bodiesSquareIndexs[i]) {
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
		for each(int squareIndex in bodiesSquareIndexs[i]) {
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

Body& Grid::getBodyAtIndex(int index) {
	return *(allBodies[index]);
}

QVector<BodiesVector*> Grid::getBodySourroundingBodiesVectors(int bodyIndex) {
	auto indexes = bodiesSquareIndexs[bodyIndex];
	auto squaresVector = QVector<BodiesVector*>(indexes.length());
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
		assert(MathUtilities::isEqual(fmod(this->sizeInMeters().width(), this->squareSideInMeters()), 0));
		assert(MathUtilities::isEqual(fmod(this->sizeInMeters().height(), this->squareSideInMeters()), 0));

		_sizeInSquares = this->transformFromMetersToSquares(this->sizeInMeters()).toSize();
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
const QSizeF& Grid::sizeInMeters() const {
	return _sizeInMeters;
}

void Grid::sizeInMeters(const QSizeF& sizeInMeters) {
	_sizeInMeters = sizeInMeters;
	resetSquareCoordinates();
}

// the side of the square this grid gets
// divided into, in meters
qreal Grid::squareSideInMeters() {
	return _squareSideInMeters;
}

void Grid::setSquareSideInMeters(qreal squareSideInMeters) {
	_squareSideInMeters = squareSideInMeters;
	resetSquareCoordinates();
}

void Grid::resetSquareCoordinates() {
	_sizeInSquares = QSize();
	_numSquares = 0;
	_rectInSquares = QRect();
}

inline QRect Grid::getBodySquareDimensions(Body const & body) {
	return this->transformFromMetersToSquares(body.boundingRect).toAlignedRect();
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
	return this->metersDimensionsSquareIndexs(body.boundingRect);
}

inline QVector<int> Grid::metersDimensionsSquareIndexs(QRectF const & metersDimensions) {
	auto squaresDimensions = this->transformFromMetersToSquares(metersDimensions).toAlignedRect();
	return this->squareDimensionsSquareIndexs(squaresDimensions);
}


inline QVector<int> Grid::squareDimensionsSquareIndexs(QRect const & squareDimensions) {
	int squaresInDimensions = squareDimensions.width() * squareDimensions.height();

	auto indexesArray = QVector<int>(squaresInDimensions);

	auto topPoint = squareDimensions.topLeft();
	auto bottomPoint = squareDimensions.bottomRight();
	int squareIndex = 0;
	if (squaresInDimensions == 1) {
		indexesArray[0] = topPoint.x() + topPoint.y() * this->sizeInSquares().width();
	}
	if (squaresInDimensions >= 2) {
		for (int i = topPoint.x(); i <= bottomPoint.x(); i++) {
			for (int j = topPoint.y(); j <= bottomPoint.y(); j++)
			{
				indexesArray[squareIndex] = i + j * this->sizeInSquares().width();
				squareIndex++;
			}
		}
	}
	return indexesArray;
}

inline QRectF Grid::transformFromMetersToSquares(QRectF const & metersRect) {
	return QRectF(transformFromMetersToSquares(metersRect.topLeft()),
		transformFromMetersToSquares(metersRect.bottomRight()));
}

inline QSizeF Grid::transformFromMetersToSquares(QSizeF const & size) {
	return QSizeF(this->transformFromMetersToSquares(size.width()),
		this->transformFromMetersToSquares(size.height()));
}

inline QPointF Grid::transformFromMetersToSquares(QPointF const & point) {
	auto p1 = this->transformFromMetersToSquares(point.x());
	auto p2 = this->transformFromMetersToSquares(point.y());
	auto p = QPointF(p1, p2);
	return p;
}

inline qreal Grid::transformFromMetersToSquares(qreal const & meters) {
	if (this->squareSideInMeters() == 0)
		return 0;
	return meters / this->squareSideInMeters();
}
