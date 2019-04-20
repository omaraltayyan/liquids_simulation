#include "Grid.h"
using namespace std;
Grid::Grid()
{
}


Grid::~Grid()
{
}

QSize Grid::getSquaresSize() {
	// both grid dimensions must be a multiple
	// of the side of the squares we divide this
	// grid into
	_ASSERT(this->size.width() % squareSide == 0);
	_ASSERT(this->size.height() % squareSide == 0);

	return QSize(this->size.width() / squareSide, this->size.height() / squareSide);
}

/*Vector2<double> getBodySquareDimensions(Body const & body) {
	body.boundingRect
}*/
