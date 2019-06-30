#include "Grid.h"
#include "Utilities\Memory.h"
using namespace std;
#include <cmath>
#include "Utilities\MathUtilities.h"
#include "Bodies\FluidParticle.h"
#include <assert.h>     /* assert */

Grid::Grid(const QSizeF& sizeInMeters, qreal squareSideInMeters)
{
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

BodiesVector Grid::getAllBodies() {
	return allBodies;
}

void Grid::addBodiesToGrid(const BodiesVector& bodies) {

	if (bodies.length() == 0)
	{
		return;
	}

	if (squaresBodies.length() == 0) {
		squaresBodies = QVector<BodiesVector*>(this->numSquares());
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i] = new BodiesVector();
		}
	}

	allBodies.append(bodies);

	auto squaresBodiesCounts = new int[this->numSquares()]();

	for (int i = allBodies.length() - bodies.length(); i < allBodies.length(); i++)
	{
		auto body = allBodies[i];

		bool rejectBody = false;
		auto bodySquareIndexes = this->getBodySquareIndexs(*body);

		if (bodySquareIndexes.length() == 0 || !this->boundingRectMeters().contains(body->boundingRect)) {
			rejectBody = true;
		}
		else {
			if (body->bodyType == fluid && squaresBodies.length() > 0)
			{
				auto particle = static_cast<FluidParticle*>(body);
				QVector<int>& indexes = this->getBodySquareIndexsWithSurroundingSquares(*body);
				for (int i = 0; i < indexes.length(); i++)
				{
					int index = indexes[i];
					for (int j = 0; j < squaresBodies[index]->length(); j++)
					{
						Body* otherBody = squaresBodies[index]->at(j);
						if (otherBody->bodyType == fluid)
						{
							auto otherParticle = static_cast<FluidParticle*>(otherBody);
							double rejectDistanceThreshold = 0.02;
							if (particle->positionVector.distanceToPoint(otherParticle->positionVector) < rejectDistanceThreshold) {
								rejectBody = true;
								goto loopEnd;
							}
						}
					}
				}
			}
		}
		loopEnd:;
		if (rejectBody)
		{
			delete allBodies.at(i);
			allBodies.removeAt(i);
			i--;
			continue;
		}

		for each(int squareIndex in bodySquareIndexes) {
			squaresBodies[squareIndex]->push_back(body);
		}
	}

	/*
	for (int i = 0; i < this->numSquares(); i++)
	{
		squaresBodies[i]->reserve(squaresBodies[i]->length() + squaresBodiesCounts[i]);
	}

	int squaresIndex = 0;
	for (int i = allBodies.length() - bodiesSquareIndexs.length(); i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		for each(int squareIndex in bodiesSquareIndexs[squaresIndex]) {
			squaresBodies[squareIndex]->push_back(body);
		}
		squaresIndex++;
	}

	*/
#ifdef _DEBUG
	int c = 0;
	for (int i = 0; i < this->numSquares(); i++)
	{
		for (int j = 0; j < squaresBodies[i]->length(); j++)
		{
			for (int z = j + 1; z < squaresBodies[i]->length(); z++)
			{
				Body& body1 = *(squaresBodies[i]->at(j));
				Body& body2 = *(squaresBodies[i]->at(z));
				if (body1.bodyType == fluid && body2.bodyType == fluid) {
					Particle& p1 = static_cast<Particle&>(body1);
					Particle& p2 = static_cast<Particle&>(body2);
					if (MathUtilities::isEqual(p1.position.x(), p2.position.x()) &&
						MathUtilities::isEqual(p1.position.y(), p2.position.y())) {
						c++;
					}
				}
			}
		}
	}
	if (c > 0)
	{
		printf("%d shared Positions found\n", c);
	}

#endif


	delete[] squaresBodiesCounts;
}

void Grid::clearBodies()
{
	for each(auto body in this->allBodies) {
		delete body;
	}
	allBodies.clear();
	allBodies.squeeze();
	for (int i = 0; i < this->numSquares(); i++)
	{
		squaresBodies[i]->clear();
		squaresBodies[i]->squeeze();
	}
}

void Grid::updateBodiesInGrid() {

	if (squaresBodies.length() == 0) {
		squaresBodies = QVector<BodiesVector*>(this->numSquares());
		for (int i = 0; i < this->numSquares(); i++)
		{
			squaresBodies[i] = new BodiesVector();
		}
	}

	auto squaresBodiesCounts = new int[this->numSquares()]();

	QVector<QVector<int>> bodiesSquareIndexs;

	bodiesSquareIndexs.resize(allBodies.length());

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

	for (int i = 0; i < this->numSquares(); i++)
	{
		squaresBodies[i]->clear();
		squaresBodies[i]->reserve(squaresBodiesCounts[i]);

	}

	for (int i = 0; i < allBodies.length(); i++)
	{
		auto body = allBodies[i];
		for each(int squareIndex in bodiesSquareIndexs[i]) {
				squaresBodies[squareIndex]->push_back(body);
		}
	}

#ifdef _DEBUG
	int c = 0;
	for (int i = 0; i < this->numSquares(); i++)
	{
		for (int j = 0; j < squaresBodies[i]->length(); j++)
		{
			for (int z = j + 1; z < squaresBodies[i]->length(); z++)
			{
				Body& body1 = *(squaresBodies[i]->at(j));
				Body& body2 = *(squaresBodies[i]->at(z));
				if (body1.bodyType == fluid && body2.bodyType == fluid) {
					Particle& p1 = static_cast<Particle&>(body1);
					Particle& p2 = static_cast<Particle&>(body2);
					if (MathUtilities::isEqual(p1.position.x(), p2.position.x()) &&
						MathUtilities::isEqual(p1.position.y(), p2.position.y())) {
						c++;
					}
				}
			}
		}
	}
	if (c > 0)
	{
		printf("%d shared Positions found\n", c);
	}

	int bodiesInSquares = 0;
	for (int i = 0; i < this->numSquares(); i++)
	{
		bodiesInSquares += squaresBodies[i]->length();
	}

	int bodiesInIndexes = 0;
	for (int i = 0; i < bodiesSquareIndexs.length(); i++)
	{
		bodiesInIndexes += bodiesSquareIndexs[i].length();
	}

	assert(bodiesInIndexes == bodiesInSquares);
#endif

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
	auto indexes = getBodySquareIndexsWithSurroundingSquares(*allBodies.at(bodyIndex));
	auto squaresVector = QVector<BodiesVector*>(indexes.length());
	for (int i = 0; i < indexes.length(); i++)
	{
		squaresVector[i] = squaresBodies.at(indexes[i]);
	}
	return squaresVector;
}


const QRect& Grid::rectInSquares() {
	if (!_rectInSquares.isValid()) {
		_rectInSquares = QRect(0, 0, this->sizeInSquares().width(), this->sizeInSquares().height());
	}

	return _rectInSquares;
}

const QRectF & Grid::boundingRectMeters()
{
	return _boundingRectMeters;
	// TODO: insert return statement here
}

const QSize& Grid::sizeInSquares() {
	if (!_sizeInSquares.isValid()) {
		// both grid dimensions must be a multiple
		// of the side of the squares we divide this
		// grid into
		int squaresOverWidth = this->sizeInMeters().width() / this->squareSideInMeters();
		int squaresOverHeight = this->sizeInMeters().height() / this->squareSideInMeters();
		this->sizeInMeters(QSizeF(squaresOverWidth * this->squareSideInMeters(), squaresOverHeight * this->squareSideInMeters()));
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
	double offset = 0.01;
	this->_boundingRectMeters = QRectF(0, 0, sizeInMeters.width(), sizeInMeters.height())
		.marginsRemoved(QMarginsF(offset, offset, offset, offset));
	resetSquareCoordinates();
}

// the side of the square this grid gets
// divided into, in meters
qreal Grid::squareSideInMeters() {
	return _squareSideInMeters;
}

void Grid::setSquareSideInMeters(qreal squareSideInMeters) {
	_squareSideInMeters = squareSideInMeters;
	kernelsInfo = KernelsInfo(_squareSideInMeters);
	resetSquareCoordinates();
}

const KernelsInfo & Grid::getKernelsInfo()
{
	return this->kernelsInfo;
}

void Grid::resetSquareCoordinates() {
	_sizeInSquares = QSize();
	_numSquares = 0;
	_rectInSquares = QRect();
}

inline QRect Grid::getBodySquareDimensions(Body const & body) {
	auto result = this->transformFromMetersToSquares(body.boundingRect).toAlignedRect();
	if (body.bodyType == fluid) {
		result.setWidth(1);
		result.setHeight(1);
	}
	return result;
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
	auto dimentions = this->metersDimensionsSquareIndexs(body.boundingRect);
	if (body.bodyType == fluid && dimentions.length() > 1) {
		dimentions.remove(1, dimentions.length() - 1);
	}
	return dimentions;
}

inline QVector<int> Grid::metersDimensionsSquareIndexs(QRectF const & metersDimensions) {
	auto squaresDimensions = this->transformFromMetersToSquares(metersDimensions).toAlignedRect();
	return this->squareDimensionsSquareIndexs(squaresDimensions);
}


inline QVector<int> Grid::squareDimensionsSquareIndexs(QRect const & squareDimensions) {
	int squaresInDimensions = squareDimensions.width() * squareDimensions.height();
	auto numberOfSquares = this->numSquares();

	auto indexesArray = QVector<int>();
	auto topPoint = squareDimensions.topLeft();
	auto bottomPoint = squareDimensions.bottomRight();
	if (squaresInDimensions == 1) {
		int squareIndex = topPoint.x() + topPoint.y() * this->sizeInSquares().width();
		if (squareIndex >= 0 && squareIndex < numberOfSquares) {
			return QVector<int> { squareIndex };
		}
	}
	if (squaresInDimensions >= 2) {
		indexesArray.reserve(squaresInDimensions);

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

#ifdef _DEBUG
	assert(!indexesArray.contains(this->numSquares()));
#endif
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
