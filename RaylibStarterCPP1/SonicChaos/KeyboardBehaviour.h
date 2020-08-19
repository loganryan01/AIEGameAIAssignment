/*--------------------------------------------------------------------
	File Name: KeyboardBehaviour.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once
#include "Behaviour.h"

class KeyboardBehaviour : public Behaviour
{
public:
	// Constructor
	KeyboardBehaviour() {};

	// Destructor
	virtual ~KeyboardBehaviour() {};

	// Update the force applied to the agent
	virtual Vector2 Update(Agent* agent, float deltaTime);

private:
	// Speed of the agent using this behaviour
	float m_speedIncrement = 100.0f;
};

