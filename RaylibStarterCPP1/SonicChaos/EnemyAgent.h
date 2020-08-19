/*--------------------------------------------------------------------
	File Name: EnemyAgent.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once
#include "Agent.h"

class EnemyAgent : public Agent
{
public:
	// Constructor
	EnemyAgent();

	// Destructor
	virtual ~EnemyAgent();

	// Update the enemy agent
	virtual void Update(float deltaTime);

	// Draw the enemy agent
	virtual void Draw();

	// Get the width of the texure of the enemy agent
	const int& GetWidth() { return m_texture.width; }

	// Get the height of the texure of the enemy agent
	const int& GetHeight() { return m_texture.height; }

	// Get the top part of the collision square around the enemy agent
	const Rectangle& GetTopAABB() { return m_topAABB; }

	// Get the left part of the collision square around the enemy agent
	const Rectangle& GetLeftAABB() { return m_leftAABB; }

	// Get the right part of the collision square around the enemy agent
	const Rectangle& GetRightAABB() { return m_rightAABB; }

	// Get the bottom part of the collision square around the enemy agent
	const Rectangle& GetBottomAABB() { return m_bottomAABB; }

private:
	// The texture for the enemy agent
	Texture2D m_texture;

	// The top part of the collision square around the enemy agent
	Rectangle m_topAABB;

	// The left part of the collision square around the enemy agent
	Rectangle m_leftAABB;

	// The right part of the collision square around the enemy agent
	Rectangle m_rightAABB;

	// The bottom part of the collision square around the enemy agent
	Rectangle m_bottomAABB;
};