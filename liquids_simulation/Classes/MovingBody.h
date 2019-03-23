#pragma once
#include "Body.h"
class MovingBody :
	public Body
{
public:

	virtual void manageCollision(MovingBody* other) = 0;

	MovingBody();
	~MovingBody();
};

