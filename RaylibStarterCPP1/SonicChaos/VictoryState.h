/*--------------------------------------------------------------------
	File Name: VictoryState.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

#include "IGameState.h"

#include "raylib.h"
#include "Button.h"

class Application;

class VictoryState : public IGameState
{
public:
	// Constructor
	VictoryState(Application* app);

	// Destructor
	virtual ~VictoryState();

	// Load the assets needed for this state
	virtual void Load();

	// Unload the assets that was used for this state
	virtual void Unload();

	// Update the state
	virtual void Update(float dt);

	// Draw the state
	virtual void Draw();

private:
	// Pointer to the application
	Application* m_app;

	// Texture of Super Sonic
	Texture2D m_superSonic;

	// Button to allow player to exit the game
	Button* m_exitButton = nullptr;
};

