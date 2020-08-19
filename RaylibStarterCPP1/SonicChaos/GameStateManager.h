/*--------------------------------------------------------------------
	File Name: GameStateManager.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

#include <map>
#include <list>
#include <functional>

class IGameState;

class GameStateManager
{
public:
	// Constructor
	GameStateManager();

	// Destructor
	~GameStateManager();

	// Update the Game State Manager
	void Update(float deltaTime);

	// Draw the Game State
	void Draw();

	// Get the stack of the Game State Manager
	const std::list<IGameState*>& GetStack() { return m_stack; }

	// Create a state for the Game State Manager stack
	void SetState(const char* name, IGameState* state);

	// Add a state to the Game State Manager stack
	void PushState(const char* name);

	// Remove a state from the Game State Manager stack
	void PopState();

protected:
	// Game States
	std::map<const char*, IGameState*> m_states;

	// Stack for the Game States
	std::list<IGameState*> m_stack;

	// Commands for the Game States
	std::list < std::function<void()> > m_commands;
};

