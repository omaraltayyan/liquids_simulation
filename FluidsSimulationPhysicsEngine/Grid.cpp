#include "Grid.h"
#include "Utilities\Memory.h"
using namespace std;
#include <cmath>
#include "Utilities\MathUtilities.h"
#include <assert.h>     /* assert */

Grid::Grid(const QSizeF& sizeInMeters, qreal squareSideInMeters)
{
	this->sizeInMeters(sizeInMeters);
	this->squareSideInMeters(squareSideInMeters);
}
Grid::~Grid()
{
	if (squaresBodies.count() > 0) {
		for (int i = 0; i < squaresBodies.count(); i++)
		{
			delete squaresBodies[i];
		}
		squaresBodies.clear();
	}

}

void Grid::addBodiesToGrid(const BodiesVector& bodies) {

	allBodies.append(bodies);

	auto squaresBodiesCounts = new int[this->numSquares()]();

	auto bodiesSquareIndexs = new QVector<int>[bodies.length()];

	for(int i = 0; i < bodies.length(); i++)
	{
		auto body = bodies[i];
		bodiesSquareIndexs[i] = this->metersDimensionsSquareIndexs(body->boundingRect);
		for each(int squareIndex in bodiesSquareIndexs[i]) {
			squaresBodiesCounts[squareIndex]++;
		}
	}

	if (squaresBodies.count() == 0) {
		squaresBodies = QVector<BodiesVector*>(this->numSquares());
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i] = new BodiesVector(squaresBodiesCounts[i]);
		}
	}
	else {
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i]->resize(squaresBodies[i]->size() + squaresBodiesCounts[i]);
		}
	}

	for (int i = 0; i < bodies.length(); i++)
	{
		auto body = bodies[i];
		for each(int squareIndex in bodiesSquareIndexs[i]) {
			int oldBodiesOffset = squaresBodies[squareIndex]->length() - squaresBodiesCounts[i];
			squaresBodies[squareIndex]->push_back(body);
		}
	}

	delete squaresBodiesCounts;
	delete bodiesSquareIndexs;
}

void Grid::updateBodiesInGrid() {

	auto squaresBodiesCounts = new int[this->numSquares()]();

	auto bodiesSquareIndexs = new QVector<int>[allBodies.length()];

	for (int i = 0; i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		bodiesSquareIndexs[i] = this->metersDimensionsSquareIndexs(body->boundingRect);
		for each(int squareIndex in bodiesSquareIndexs[i]) {
			squaresBodiesCounts[squareIndex]++;
		}
	}

	if (squaresBodies.count() == 0) {
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
			squaresBodies[i]->resize(squaresBodiesCounts[i]);
		}
	}

	for (int i = 0; i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		for each(int squareIndex in bodiesSquareIndexs[i]) {
			int oldBodiesOffset = squaresBodies[squareIndex]->length() - squaresBodiesCounts[i];
			squaresBodies[squareIndex]->push_back(body);
		}
	}

	delete squaresBodiesCounts;
	delete bodiesSquareIndexs;
}

const QRectF& Grid::rectInSquares() {
	if (_rectInSquares.isNull()) {
		_rectInSquares = QRectF(0, 0, this->sizeInSquares().width(), this->sizeInSquares().height());
	}

	return _rectInSquares;
}

const QSizeF& Grid::sizeInSquares() {
	if (_sizeInSquares.isNull()) {
		// both grid dimensions must be a multiple
		// of the side of the squares we divide this
		// grid into
		assert (MathUtilities::isEqual(fmod(this->sizeInMeters().width(), this->squareSideInMeters()), 0));
		assert (MathUtilities::isEqual(fmod(this->sizeInMeters().height(), this->squareSideInMeters()), 0));

		_sizeInSquares = this->transformFromMetersToSquares(this->sizeInMeters());
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

void Grid::squareSideInMeters(qreal squareSideInMeters) {
	_squareSideInMeters = squareSideInMeters;
	resetSquareCoordinates();
}

void Grid::resetSquareCoordinates() {
	_sizeInSquares = QSize();
	_numSquares = 0;
	_rectInSquares = QRectF();
}

inline QRectF Grid::getBodySquareDimensions(Body const & body) {
	return this->transformFromMetersToSquares(body.boundingRect);
}

QRectF Grid::getBodySquareDimentionsWithSurroundingSquares(Body const & body) {
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
	auto squaresDimensions = this->transformFromMetersToSquares(metersDimensions);
	return this->squareDimensionsSquareIndexs(squaresDimensions);
}


inline QVector<int> Grid::squareDimensionsSquareIndexs(QRectF const & squareDimensions) {
	int squaresInDimensions = squareDimensions.width() * squareDimensions.height();

	auto indexesArray = QVector<int>(squaresInDimensions);

	auto topPoint = squareDimensions.topLeft();
	auto bottomPoint = squareDimensions.bottomRight();
	int squareIndex = 0;
	if (squaresInDimensions == 1) {
		indexesArray[0] = topPoint.x() * this->sizeInSquares().width() + topPoint.y();
	}
	if (squaresInDimensions >= 2) {
		for (int i = topPoint.x(); i <= bottomPoint.x(); i++) {
			for (int j = topPoint.y(); j <= bottomPoint.y(); j++)
			{
				indexesArray[squareIndex] = i * this->sizeInSquares().width() + j;
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

	return QPointF(this->transformFromMetersToSquares(point.x()),
		this->transformFromMetersToSquares(point.y()));
}

inline qreal Grid::transformFromMetersToSquares(qreal const & meters) {
	if (this->squareSideInMeters() == 0)
		return 0;
	return meters / this->squareSideInMeters();
}