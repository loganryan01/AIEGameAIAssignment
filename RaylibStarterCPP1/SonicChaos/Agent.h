/*--------------------------------------------------------------------
	File Name: Agent.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

#include "raymath.h"
#include <vector>

class Behaviour;

class Agent
{
public:
	// Constructor
	Agent();

	// Destructor
	virtual ~Agent();

	// Update the agent and its behaviour
	virtual void Update(float deltaTime);

	// Draw the agent
	virtual void Draw();

	// Add a behaviour to the agent
	void SetBehaviour(Behaviour* behaviour) { m_behaviour = behaviour; }

	// Get the behaviour of the agent
	const Behaviour* GetBehaviour() { return m_behaviour; }

	// Set position of the agent
	void SetPosition(Vector2 position) { m_position = position; }

	// Get the position of the agent
	const Vector2& GetPosition() { return m_position; }

	// Set the velocity of the agent
	void SetVelocity(Vector2 velocity) { m_velocity = velocity; }

	// Get the velocity of the agent
	const Vector2& GetVelocity() { return m_velocity; }

	// Set the attack radius of the agent
	void SetAttackRadius(float radius) { m_attackRadius = radius; }

	// Get the attack radius of the agent
	const float& GetAttackRadius() { return m_attackRadius; }

	// Increase the attack charger by 1
	void IncrementAttackCharger() { m_attackCharger++; }

	// Reset the attack charger to 0
	void ResetAttackCharger() { m_attackCharger = 0.0f; }

	// Get the attack charger of the agent
	const float& GetAttackCharger() { return m_attackCharger; }

protected:
	// The behaviour of the agent
	Behaviour* m_behaviour = nullptr;

	// The position of the agent
	Vector2 m_position = { 0, 0 };

	// The velocity of the agent
	Vector2 m_velocity = { 0, 0 };

	// The attack charger of the agent
	float m_attackCharger = 0.0f;

	// The attack radius of the agent
	float m_attackRadius = 25.0f;
};

