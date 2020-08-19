/*--------------------------------
	File Name: EnemyAgent.cpp
	Purpose: Create a enemy agent.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------*/

#include "EnemyAgent.h"

//------------
// Constructor
//------------
EnemyAgent::EnemyAgent()
{
	// Load the Shadow Texture
	m_texture = LoadTexture("./resources/Shadow.png");

	// Create an AABB for the enemy agent
	m_topAABB = { m_position.x + 1.0f, m_position.y, (float)m_texture.width - 1.0f, 1.0f };
	m_leftAABB = { m_position.x, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f };
	m_rightAABB = { m_position.x + (float)m_texture.width, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f };
	m_bottomAABB = { m_position.x + 1.0f, m_position.y + ((float)m_texture.height - 1.0f), (float)m_texture.width - 1.0f, 1.0f };
}

//-----------
// Destructor
//-----------
EnemyAgent::~EnemyAgent()
{
	UnloadTexture(m_texture);
}

//--------------------------------------------------------------
// Update the enemy agent
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void EnemyAgent::Update(float deltaTime)
{
	// Update the AABB for the enemy agent
	m_topAABB = { m_position.x + 1.0f, m_position.y, (float)m_texture.width - 1.0f, 1.0f };
	m_leftAABB = { m_position.x, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f };
	m_rightAABB = { m_position.x + (float)m_texture.width, m_position.y + 1.0f, 1.0f, (float)m_texture.height - 2.0f };
	m_bottomAABB = { m_position.x + 1.0f, m_position.y + ((float)m_texture.height - 1.0f), (float)m_texture.width - 1.0f, 1.0f };

	Agent::Update(deltaTime);
}

//---------------------
// Draw the enemy agent
//---------------------
void EnemyAgent::Draw()
{
	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}