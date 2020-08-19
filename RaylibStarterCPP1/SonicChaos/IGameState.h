/*--------------------------------------------------------------------
	File Name: IGameState.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

class IGameState
{
public:
	// Constructor
	IGameState() {};

	// Destructor
	virtual ~IGameState() {};

	// Load the game state
	virtual void Load() {};

	// Unload the game state
	virtual void Unload() {};

	// Update the game state
	virtual void Update(float dt) {};

	// Draw the game state
	virtual void Draw() {};
};