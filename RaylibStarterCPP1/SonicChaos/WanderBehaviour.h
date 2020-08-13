#pragma once
#include "Behaviour.h"

#include <list>

class WanderBehaviour : public Behaviour
{
public:
	WanderBehaviour() {};
	virtual ~WanderBehaviour() {};

	virtual Vector2 Update(Agent* agent, float deltaTime);

	void SetTarget(Vector2 position) { m_target = position; }
	Vector2 GetTarget() { return m_target; }

	void SetPathNode(Vector2 position) { m_myPath.push_back(position); }

private:
	Vector2 Normalise(Vector2 vector2);

	float m_maxVelocity = 100.0f;
	Vector2 m_target = { 0, 0 };

	std::list<Vector2> m_myPath;
};
