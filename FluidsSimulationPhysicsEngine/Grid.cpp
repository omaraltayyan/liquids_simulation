#include "Grid.h"
#include "Utilities\Memory.h"
using namespace std;
Grid::Grid()
{
	squaresBodies = NULL;
}
Grid::~Grid()
{
	MemoryUtilities::delete_var<BodiesVector>(squaresBodies);
}

void Grid::addBodiesToGrid(const QVector<Body*>& bodies) {
	auto squaresBodiesCounts = new int[this->numSquares()]();

	auto bodiesSquareNumbers = new QVector<int>[bodies.length()];

	for(int i = 0; i < bodies.length(); i++)
	{
		auto body = bodies[i];
		bodiesSquareNumbers[i] = this->metersDimensionsSquareNumbers(body->boundingRect);
		for each(int squareNumber in bodiesSquareNumbers[i]) {
			squaresBodiesCounts[squareNumber]++;
		}
	}

	if (squaresBodies == NULL) {
		squaresBodies = new BodiesVector[this->numSquares()];
	}

	for (int i = 0; i < this->numSquares(); i++)
	{
		squaresBodies[i].resize(squaresBodies[i].size() + squaresBodiesCounts[i]);
	}

	for (int i = 0; i < bodies.length(); i++)
	{
		auto body = bodies[i];
		for each(int squareNumber in bodiesSquareNumbers[i]) {
			int oldBodiesOffset = squaresBodies[squareNumber].length() - squaresBodiesCounts[i];
			squaresBodies[squareNumber][oldBodiesOffset + i] = body;
		}
	}

	delete squaresBodiesCounts;
	delete bodiesSquareNumbers;
}

const QRect& Grid::rectInSquares() {
	if (_rectInSquares.isNull()) {
		_rectInSquares = QRect(0, 0, this->sizeInSquares().width(), this->sizeInSquares().height());
	}

	return _rectInSquares;
}

const QSize& Grid::sizeInSquares() {
	if (_sizeInSquares.isNull()) {
		// both grid dimensions must be a multiple
		// of the side of the squares we divide this
		// grid into
		_ASSERT(this->sizeInMeters().width() % this->squareSideInMeters() == 0);
		_ASSERT(this->sizeInMeters().height() % this->squareSideInMeters() == 0);

		_sizeInSquares = this->transformFromMetersToSquares(this->sizeInMeters());
	}
	return _sizeInSquares;
}

int Grid::numSquares() {
	return this->sizeInSquares().height() * this->sizeInSquares().width();
}

// the grid window size
const QSize& Grid::sizeInMeters() const {
	return _sizeInMeters;
}

void Grid::sizeInMeters(const QSize& sizeInMeters) {
	_sizeInMeters = sizeInMeters;
	resetSquareCoordinates();
}

// the side of the square this grid gets
// divided into, in meters
int Grid::squareSideInMeters() {
	return _squareSideInMeters;
}

void Grid::squareSideInMeters(int squareSideInMeters) {
	_squareSideInMeters = squareSideInMeters;
	resetSquareCoordinates();
}

void Grid::resetSquareCoordinates() {
	_sizeInSquares = QSize();
	_rectInSquares = QRect();
}

inline QRect Grid::getBodySquareDimensions(Body const & body) {
	return this->transformFromMetersToSquares(body.boundingRect);
}


inline QVector<int> Grid::metersDimensionsSquareNumbers(QRect const & metersDimensions) {
	auto squaresDimensions = this->transformFromMetersToSquares(metersDimensions);
	return this->squareDimensionsSquareNumbers(squaresDimensions);
}


inline QVector<int> Grid::squareDimensionsSquareNumbers(QRect const & squareDimensions) {
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

inline QRect Grid::transformFromMetersToSquares(QRect const & metersRect) {
	return QRect(transformFromMetersToSquares(metersRect.topLeft()),
		transformFromMetersToSquares(metersRect.bottomRight()));
}

inline QSize Grid::transformFromMetersToSquares(QSize const & size) {
	return QSize(this->transformFromMetersToSquares(size.width()),
		this->transformFromMetersToSquares(size.height()));
}

inline QPoint Grid::transformFromMetersToSquares(QPoint const & point) {

	return QPoint(this->transformFromMetersToSquares(point.x()),
		this->transformFromMetersToSquares(point.y()));
}

inline int Grid::transformFromMetersToSquares(int const & meters) {
	if (this->squareSideInMeters() == 0)
		return 0;
	return meters / this->squareSideInMeters();
}