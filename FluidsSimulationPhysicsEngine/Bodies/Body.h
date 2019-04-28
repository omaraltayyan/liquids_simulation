#pragma once
#include <QRect>

class Body
{
public:

	virtual void draw() = 0;

	QRectF boundingRect;

	Body();
	~Body();
};

