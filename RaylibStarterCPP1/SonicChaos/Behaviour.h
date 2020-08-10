#pragma once
#include "Agent.h"

class Behaviour
{
public:
	Behaviour() {}
	virtual ~Behaviour() {}

	virtual Vector2 Update(Agent* agent, float deltaTime) = 0;
};

