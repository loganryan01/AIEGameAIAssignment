/*--------------------------------------------------------------------
	File Name: Behaviour.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once
#include "Agent.h"

class Behaviour
{
public:
	// Constructor
	Behaviour() {}

	// Destructor
	virtual ~Behaviour() {}

	// Update the force applied to the agent
	virtual Vector2 Update(Agent* agent, float deltaTime) = 0;
};

