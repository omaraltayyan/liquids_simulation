#pragma once
#include "Body.h"
class MovingBody :
	public Body
{
public:

	virtual void manageCollision(Body* other) = 0;

	MovingBody();
	~MovingBody();
};

