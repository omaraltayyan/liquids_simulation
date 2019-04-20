#include "Grid.h"
using namespace std;
Grid::Grid()
{
}


Grid::~Grid()
{
}

Vector2<int> Grid::getSquaresSize() {
	// both grid dimensions must be a multiple
	// of the side of the squares we divide this
	// grid into
	_ASSERT(size[0] % squareSide == 0);
	_ASSERT(size[1] % squareSide == 0);

	auto result = Vector2<int>();
	result[0] = size[0] / squareSide;
	result[1] = size[1] / squareSide;
	return result;
}

/*Vector2<double> getBodySquareDimensions(Body const & body) {
	body.boundingRect
}*/
