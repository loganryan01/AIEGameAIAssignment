/*-------------------------------------------------------
	File Name: SeekMasterEmerald.cpp
	Purpose: Get the steering force needed for the agent.
	Author: Logan Ryan
	Modified: 19 August 2020
---------------------------------------------------------
	Copyright 2020 Logan Ryan.
-------------------------------------------------------*/

#include "SeekMasterEmerald.h"

//----------------------------------------------------
// Normalise a vector
//	vector2 (Vector2): What vector are we normalising?
//----------------------------------------------------
const Vector2& SeekMasterEmerald::Normalise(Vector2 vector2)
{
	// Create a vector2 for the normalised vector
	Vector2 normalisedVector = { 0, 0 };

	// If there is no vector return an empty vector
	if (vector2.x == NULL && vector2.y == NULL)
		return normalisedVector;

	// Set the normalised vector to be the vector we are normalising
	normalisedVector = vector2;

	// Get the magnitude of the vector
	float magnitude = sqrtf((normalisedVector.x * normalisedVector.x) + (normalisedVector.y * normalisedVector.y));

	// Divide the normalised vector by the magnitude
	normalisedVector.x /= magnitude;
	normalisedVector.y /= magnitude;

	// Return the normalised vector
	return normalisedVector;
}

//--------------------------------------------------------------------
// Update the force applied to the agent
//	agent (Agent*): Which agent are we applying the steering force to?
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------------
Vector2 SeekMasterEmerald::Update(Agent* agent, float deltaTime)
{
	Vector2 desiredVelocity = { 0, 0 };
	Vector2 steering = { 0, 0 };

	// Calculate the desired velocity:
	// - Normalise the difference between the target's position and the agent's position
	// - Multiply it by the maximum velocity
	desiredVelocity = Vector2Scale(Normalise(Vector2Subtract(m_target, agent->GetPosition())), m_maxVelocity);

	// Calculate the steering force:
	// - Subtract the desired velocity by the agent's velocity
	steering = Vector2Subtract(desiredVelocity, agent->GetVelocity());

	// The distance between the target's position and the agent's position
	float distance = Vector2Distance(agent->GetPosition(), m_target);

	// If the agent is on the node and the behaviour does have a path
	if (distance < 16.0f && !m_myPath.empty())
	{
		// Remove the first node on the path
		m_myPath.pop_front();

		// If the path is empty, return 0 force
		if (m_myPath.empty())
		{
			return { 0,0 };
		}

		// Set the target to be the next node
		SetTarget(m_myPath.front());
	}

	// Return the steering force
	return steering;
}