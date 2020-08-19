/*-------------------------------------------------------
	File Name: KeyboardBehaviour.cpp
	Purpose: Get the steering force needed for the agent.
	Author: Logan Ryan
	Modified: 19 August 2020
---------------------------------------------------------
	Copyright 2020 Logan Ryan.
-------------------------------------------------------*/


#include "KeyboardBehaviour.h"

//--------------------------------------------------------------------
// Update the force applied to the agent
//	agent (Agent*): Which agent are we applying the steering force to?
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------------
Vector2 KeyboardBehaviour::Update(Agent* agent, float deltaTime)
{
	Vector2 force = { 0, 0 };

	// If the up key is pressed move up
	if (IsKeyDown(KEY_UP))
		force.y = -m_speedIncrement;

	// If the down key is pressed move down
	if (IsKeyDown(KEY_DOWN))
		force.y = m_speedIncrement;

	// If the left key is pressed move left
	if (IsKeyDown(KEY_LEFT))
		force.x = -m_speedIncrement;

	// If the right key is pressed move right
	if (IsKeyDown(KEY_RIGHT))
		force.x = m_speedIncrement;

	// Return the steering force
	return force;
}