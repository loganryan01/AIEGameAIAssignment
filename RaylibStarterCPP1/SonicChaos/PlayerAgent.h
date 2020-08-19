/*--------------------------------------------------------------------
	File Name: PlayerAgent.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once
#include "Agent.h"

class PlayerAgent : public Agent
{
public:
	// Constructor
	PlayerAgent();

	// Destructor
	virtual ~PlayerAgent();

	// Update the player agent
	virtual void Update(float deltaTime);

	// Draw the player agent
	virtual void Draw();

	// Get the width of the texure of the player agent
	const int& GetWidth() { return m_texture.width; }

	// Get the height of the texure of the player agent
	const int& GetHeight() { return m_texture.height; }

	// Get the top part of the collision square around the player agent
	const Rectangle& GetTopAABB() { return m_topAABB; }

	// Get the left part of the collision square around the player agent
	const Rectangle& GetLeftAABB() { return m_leftAABB; }

	// Get the right part of the collision square around the player agent
	const Rectangle& GetRightAABB() { return m_rightAABB; }

	// Get the bottom part of the collision square around the player agent
	const Rectangle& GetBottomAABB() { return m_bottomAABB; }

private:
	// The texture for the player agent
	Texture2D m_texture;

	// The top part of the collision square around the player agent
	Rectangle m_topAABB;

	// The left part of the collision square around the player agent
	Rectangle m_leftAABB;

	// The right part of the collision square around the player agent
	Rectangle m_rightAABB;

	// The bottom part of the collision square around the player agent
	Rectangle m_bottomAABB;
};

