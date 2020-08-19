/*---------------------------------
	File Name: PlayerAgent.cpp
	Purpose: Create a player agent.
	Author: Logan Ryan
	Modified: 19 August 2020
-----------------------------------
	Copyright 2020 Logan Ryan.
---------------------------------*/

#include "PlayerAgent.h"

//------------
// Constructor
//------------
PlayerAgent::PlayerAgent()
{
	// Load the Sonic texture
	m_texture = LoadTexture("./resources/Sonic.png");

	// Create an AABB for the player
	m_topAABB = { m_position.x + 1.0f, m_position.y, (float)m_texture.width - 1.0f, 1.0f };
	m_leftAABB = { m_position.x, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f };
	m_rightAABB = { m_position.x + (float)m_texture.width, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f};
	m_bottomAABB = { m_position.x + 1.0f, m_position.y + ((float)m_texture.height - 1.0f), (float)m_texture.width - 1.0f, 1.0f};
}

//-----------
// Destructor
//-----------
PlayerAgent::~PlayerAgent()
{
	UnloadTexture(m_texture);
}

//--------------------------------------------------------------
// Update the player agent
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void PlayerAgent::Update(float deltaTime)
{
	// Update the AABB for the player
	m_topAABB = { m_position.x + 1.0f, m_position.y, (float)m_texture.width - 1.0f, 1.0f };
	m_leftAABB = { m_position.x, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f };
	m_rightAABB = { m_position.x + (float)m_texture.width, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f };
	m_bottomAABB = { m_position.x + 1.0f, m_position.y + ((float)m_texture.height - 1.0f), (float)m_texture.width - 1.0f, 1.0f };
	
	Agent::Update(deltaTime);
}

//----------------------
// Draw the player agent
//----------------------
void PlayerAgent::Draw()
{
	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}