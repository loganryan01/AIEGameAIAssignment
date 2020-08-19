/*---------------------------------
	File Name: GameStateManager.cpp
	Purpose: Mange the game states.
	Author: Logan Ryan
	Modified: 19 August 2020
-----------------------------------
	Copyright 2020 Logan Ryan.
---------------------------------*/

#include "GameStateManager.h"
#include "IGameState.h"

//------------
// Constructor
//------------
GameStateManager::GameStateManager()
{

}

//-----------
// Destructor
//-----------
GameStateManager::~GameStateManager()
{
	for (auto iter = m_states.begin(); iter != m_states.end(); iter++)
	{
		if (iter->second != nullptr)
		{
			iter->second->Unload();
			delete iter->second;
		}
	}

	m_states.clear();
}

//--------------------------------------------------------------
// Update the game state manager
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void GameStateManager::Update(float deltaTime)
{
	// invoke all recorded commands
	for (auto cmd : m_commands)
	{
		cmd();
	}
	m_commands.clear();

	for (auto state : m_stack)
	{
		state->Update(deltaTime);
	}
}

//---------------------
// Draw the game states
//---------------------
void GameStateManager::Draw()
{
	for (auto state : m_stack)
	{
		state->Draw();
	}
}

//----------------------------------------------------
// Create a state for the Game State Manager stack
//	name (const char*): What is the name of the state?
//	state (IGameState*): What state is it?
//----------------------------------------------------
void GameStateManager::SetState(const char* name, IGameState* state)
{
	m_commands.push_back([=]()
		{
			if (m_states[name] != nullptr)
			{
				m_states[name]->Unload();
				delete m_states[name];
			}

			m_states[name] = state;

			if (m_states[name] != nullptr)
			{
				m_states[name]->Load();
			}
		});
}

//----------------------------------------------------
// Add a state to the Game State Manager stack
//	name (const char*): What is the name of the state?
//----------------------------------------------------
void GameStateManager::PushState(const char* name)
{
	m_commands.push_back([=]()
		{
			m_stack.push_back(m_states[name]);
		});
}

//-------------------------------------------------
// Remove a state from the Game State Manager stack
//-------------------------------------------------
void GameStateManager::PopState()
{
	m_commands.push_back([=]()
		{
			m_stack.pop_back();
		});
}