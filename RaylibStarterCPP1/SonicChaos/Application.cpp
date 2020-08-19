/*-------------------------------
	File Name: Application.cpp
	Purpose: Run the application.
	Author: Logan Ryan
	Modified: 19 August 2020
---------------------------------
	Copyright 2020 Logan Ryan.
-------------------------------*/

#include "Application.h"
#include "GameStateManager.h"
#include "IGameState.h"
#include "PlayState.h"
#include "SplashState.h"
#include "MenuState.h"
#include "VictoryState.h"
#include "DefeatState.h"

#include "raylib.h"

//--------------------------------------------------------
// Constructor
//	windowWidth (int): What is the width of the window?
//	windowHeight (int): What is the height of the window?
//--------------------------------------------------------
Application::Application(int windowWidth, int windowHeight) :
	m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{

}

//-----------
// Destructor
//-----------
Application::~Application()
{

}

//-----------------------------
// Load the assets for the game
//-----------------------------
void Application::Load()
{
	// Create the window
	InitWindow(m_windowWidth, m_windowHeight, "Sonic Chaos");

	// Create the game state manager
	m_gameStateManager = new GameStateManager();

	// Create the game states
	m_gameStateManager->SetState("Splash", new SplashState(this));
	m_gameStateManager->SetState("Menu", new MenuState(this));
	m_gameStateManager->SetState("Play", new PlayState(this));
	m_gameStateManager->SetState("Victory", new VictoryState(this));
	m_gameStateManager->SetState("Defeat", new DefeatState(this));

	// Add the splash state to stack
	m_gameStateManager->PushState("Splash");
}

//-------------
// Run the game
//-------------
void Application::Run()
{
	Load();

	// While the window is open
	while (!WindowShouldClose())
	{
		// Get the frame time
		float dt = GetFrameTime();

		// Update and draw the game
		Update(dt);
		Draw();

		// If the stack is empty, close the game
		if (m_gameStateManager->GetStack().empty())
		{
			break;
		}
	}

	// Unload the game state manager
	delete m_gameStateManager;

	CloseWindow();
}

//--------------------------------------------------------------
// Update the game
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void Application::Update(float deltaTime)
{
	m_gameStateManager->Update(deltaTime);
}

//--------------
// Draw the game
//--------------
void Application::Draw()
{
	BeginDrawing();

	// Set the background to be white
	ClearBackground(RAYWHITE);

	// Draw the game state
	m_gameStateManager->Draw();

	EndDrawing();
}