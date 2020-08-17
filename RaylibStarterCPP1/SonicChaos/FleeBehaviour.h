#pragma once
#include "Behaviour.h"

class FleeBehaviour : public Behaviour
{
public:
	FleeBehaviour() {};
	virtual ~FleeBehaviour() {};

	virtual Vector2 Update(Agent* agent, float deltaTime);

	void SetTarget(Vector2 position) { m_target = position; }
	Vector2 GetTarget() { return m_target; }

private:
	Vector2 Normalise(Vector2 vector2);

	float m_maxVelocity = 100.0f;
	Vector2 m_target = { 0, 0 };
};

