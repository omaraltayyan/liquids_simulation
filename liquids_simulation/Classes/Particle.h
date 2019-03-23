#pragma once
#include "MovingBody.h"
class Particle :
	public MovingBody
{
public:

	virtual void interactWith(Particle* other) = 0;


	Particle();
	~Particle();
};

