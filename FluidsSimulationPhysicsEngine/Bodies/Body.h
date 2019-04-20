#pragma once
#include <QRect>

class Body
{
public:

	virtual void draw() = 0;

	QRect boundingRect;

	Body();
	~Body();
};

