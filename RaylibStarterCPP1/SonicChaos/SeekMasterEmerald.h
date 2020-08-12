#pragma once
#include "Behaviour.h"

class SeekMasterEmerald : public Behaviour
{
public:
	SeekMasterEmerald() {};
	virtual ~SeekMasterEmerald() {};

	virtual Vector2 Update(Agent* agent, float deltaTime);

	void SetTarget(Vector2 position) { m_target = position; }
	Vector2 GetTarget() { return m_target; }

private:
	Vector2 Normalise(Vector2 vector2);
	void OnArrival();

	float m_maxVelocity = 100.0f;
	Vector2 m_target = { 0, 0 };
};

