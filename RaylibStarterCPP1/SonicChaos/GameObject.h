#pragma once

#include "raylib.h"

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject() {};

	virtual void Draw() {};

	// Movement functions
	void SetPosition(Vector2 position) { m_position = position; }
	Vector2 GetPosition() { return m_position; }

protected:
	Vector2 m_position = { 0, 0 };
};

