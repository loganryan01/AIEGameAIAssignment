/*--------------------------------------------------------------------
	File Name: GameObject.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

#include "raylib.h"

class GameObject
{
public:
	// Consdtructor
	GameObject() {};

	// Destructor
	virtual ~GameObject() {};

	// Draw the Game Object
	virtual void Draw() {};

	// Set the position of the Game Object
	void SetPosition(Vector2 position) { m_position = position; }

	// Get the position of the Game Object
	const Vector2& GetPosition() { return m_position; }

protected:
	// The position of the Game Object
	Vector2 m_position = { 0, 0 };
};

