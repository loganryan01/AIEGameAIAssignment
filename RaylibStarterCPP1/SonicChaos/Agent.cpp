/*-----------------------------------------
	File Name: Agent.cpp
	Purpose: Creates an agent for the game.
	Author: Logan Ryan
	Modified: 19 August 2020
-------------------------------------------
	Copyright 2020 Logan Ryan.
-----------------------------------------*/

#include "Agent.h"
#include "Behaviour.h"

//------------
// Constructor
//------------
Agent::Agent()
{}

//-----------
// Destructor
//-----------
Agent::~Agent()
{}

//--------------------------------------------------------------
// Update the agent and its behaviour
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void Agent::Update(float deltaTime)
{
	// Calculate the force of the agent
	Vector2 force = m_behaviour->Update(this, deltaTime);

	// Apply the force to the agent
	m_velocity = (Vector2Add(m_velocity, Vector2Scale(force, deltaTime)));
	m_position = (Vector2Add(m_position, Vector2Scale(m_velocity, deltaTime)));
}

//---------------
// Draw the agent
//---------------
void Agent::Draw()
{
	DrawRectangle((int)m_position.x, (int)m_position.y, 16, 16, RED);
}