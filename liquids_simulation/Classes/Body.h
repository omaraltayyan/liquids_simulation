#pragma once
#include "Mathematics\GteAlignedBox.h"
using namespace gte;

class Body
{
public:

	virtual void draw() = 0;

	AlignedBox2<double> boundingRect;



	Body();
	~Body();
};

