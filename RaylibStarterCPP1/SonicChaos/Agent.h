#pragma once
#include "raymath.h"
#include <vector>

class Behaviour;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	// Update the agent and its behaviours
	virtual void Update(float deltaTime);

	// Draw the agent
	virtual void Draw();

	// Add a behaviour to the agent
	void SetBehaviour(Behaviour* behaviour);
	Behaviour* GetBehaviour() { return m_behaviour; }

	// Movement functions
	void SetPosition(Vector2 position) { m_position = position; }
	Vector2 GetPosition() { return m_position; }
	void SetVelocity(Vector2 velocity) { m_velocity = velocity; }
	Vector2 GetVelocity() { return m_velocity; }

protected:
	Behaviour* m_behaviour = nullptr;

	Vector2 m_position = { 0, 0 };
	Vector2 m_velocity = { 0, 0 };
};

