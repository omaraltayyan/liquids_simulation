#include "GTMathematics.h"
#include "Classes\Body.h"
using namespace gte;

#pragma once
class Grid
{


public:

	// the grid window size
	Vector2<int> size;

	// the side of the square this grid gets
	// divided into
	int squareSide;

	Vector2<int> getSquaresSize();

	// returns the dimensions of the given body
	// inside the grid relative to the squares
	// coordinate system.
	// Vector2<double> getBodySquareDimensions(Body const & body);

	Grid();
	~Grid();
};

