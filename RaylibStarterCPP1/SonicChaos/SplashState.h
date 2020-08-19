/*--------------------------------------------------------------------
	File Name: SplashState.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

#include "IGameState.h"

#include "raylib.h"

class Application;

class SplashState : public IGameState
{
public:
	// Constructor
	SplashState(Application* app);

	// Destructor
	virtual ~SplashState();

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

	// SEGA logo
	Texture2D m_logo;

	// Timer for the screens
	float m_timer = 0.0f;
};

