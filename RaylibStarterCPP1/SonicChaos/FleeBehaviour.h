/*--------------------------------------------------------------------
	File Name: FleeBehaviour.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once
#include "Behaviour.h"

#include <list>

class FleeBehaviour : public Behaviour
{
public:
	// Constructor
	FleeBehaviour() {};

	// Destructor
	virtual ~FleeBehaviour() {};

	// Update the force applied to the agent
	virtual Vector2 Update(Agent* agent, float deltaTime);

	// Add a node to the path for the behaviour
	void SetPathNode(Vector2 position) { m_myPath.push_back(position); }

	// Get the path of the behaviour
	const std::list<Vector2>& GetPath() { return m_myPath; }

	// Set the target for the behaviour
	void SetTarget(Vector2 position) { m_target = position; }
	
	// Get the target of the behaviour
	const Vector2& GetTarget() { return m_target; }

private:
	// Normalise a vector
	const Vector2& Normalise(Vector2 vector2);

	// The max velocity of the behaviour
	float m_maxVelocity = 100.0f;

	// The target of the behaviour
	Vector2 m_target = { 0, 0 };

	// The path for the agent to follow
	std::list<Vector2> m_myPath;
};

