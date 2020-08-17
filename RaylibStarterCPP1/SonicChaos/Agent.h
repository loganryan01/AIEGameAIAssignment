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

	// Attack Radius
	void SetAttackRadius(float radius) { m_attackRadius = radius; }
	const float& GetAttackRadius() { return m_attackRadius; }

	// Attack Bar
	void IncrementAttackCharger() { m_attackCharger++; }
	void ResetAttackCharger() { m_attackCharger = 0.0f; }
	const float& GetAttackCharger() { return m_attackCharger; }

protected:
	Behaviour* m_behaviour = nullptr;

	Vector2 m_position = { 0, 0 };
	Vector2 m_velocity = { 0, 0 };

	float m_attackCharger = 0.0f;
	float m_attackRadius = 25.0f;
};

