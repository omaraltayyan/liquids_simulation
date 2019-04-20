#include "Bodies\Body.h"
#include <QRect>
#include <QVector2D>
#include <QSize>

#pragma once
class Grid
{


public:

	// the grid window size
	QSize size;

	// the side of the square this grid gets
	// divided into
	int squareSide;

	QSize getSquaresSize();

	// returns the dimensions of the given body
	// inside the grid relative to the squares
	// coordinate system.
	// Vector2<double> getBodySquareDimensions(Body const & body);

	Grid();
	~Grid();
};

