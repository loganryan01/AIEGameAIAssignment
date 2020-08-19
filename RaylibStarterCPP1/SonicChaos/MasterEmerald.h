/*--------------------------------------------------------------------
	File Name: MasterEmerald.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once
#include "GameObject.h"

class MasterEmerald : public GameObject
{
public:
	// Constructor
	MasterEmerald();

	// Destructor
	virtual ~MasterEmerald() {};

	// Draw the Master Emerald
	virtual void Draw();

	// Set the texture for the Master Emerald
	void SetTexture(Texture2D texture) { m_texture = texture; }

	// Get the texture for the Master Emerald
	const Texture2D& GetTexture() { return m_texture; }

	// Get the radius of the Master Emerald collision circle
	const float& GetRadius() { return m_radius; }

private:
	// The texture for the Master Emerald
	Texture2D m_texture;

	// The radius of the collision circle for the Chaos Emerald
	float m_radius = 32.0f;
};

