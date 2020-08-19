/*--------------------------------------------------------------------
	File Name: Application.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

class GameStateManager;

class Application
{
public:
	// Constructor
	Application(int windowWidth, int windowHeight);

	// Destructor
	~Application();

	// Load the assets for the game
	void Load();

	// Run the game
	void Run();

	// Update the game
	void Update(float deltaTime);

	// Draw all the pictures in the game
	void Draw();

	// Get the game state manager
	GameStateManager* GetGameStateManager()
	{
		return m_gameStateManager;
	}

private:
	// Width of the window
	int m_windowWidth;

	// Height of the window
	int m_windowHeight;

	// The manager for the game states
	GameStateManager* m_gameStateManager = nullptr;
};

