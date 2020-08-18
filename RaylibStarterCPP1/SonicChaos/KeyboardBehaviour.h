#pragma once
#include "Behaviour.h"

class KeyboardBehaviour : public Behaviour
{
public:
	KeyboardBehaviour() {};
	virtual ~KeyboardBehaviour() {};

	virtual Vector2 Update(Agent* agent, float deltaTime);

	void SetSpeed(float speed)
	{
		m_speedIncrement = speed;
	}

private:
	float m_speedIncrement = 100.0f;
};

