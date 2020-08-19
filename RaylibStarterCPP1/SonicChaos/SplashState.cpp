/*-------------------------------------------------------------------
	File Name: SplashState.cpp
	Purpose: Tell the player who created the game and the characters.
	Author: Logan Ryan
	Modified: 19 August 2020
---------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
-------------------------------------------------------------------*/

#include "SplashState.h"

#include "Application.h"
#include "GameStateManager.h"

//-----------------------------------------------------
// Constructor
//	app (Application*): Which application are we using?
//-----------------------------------------------------
SplashState::SplashState(Application* app) : m_app(app)
{
	m_logo = { 0 };
}

//-----------
// Destructor
//-----------
SplashState::~SplashState()
{

}

//--------------------------------------
// Load the assets needed for this state
//--------------------------------------
void SplashState::Load()
{
	// Load the SEGA logo
	m_logo = LoadTexture("./resources/SEGA logo.png");
}

//-----------------------------------------------
// Unload the assets that was used for this state
//-----------------------------------------------
void SplashState::Unload()
{
	UnloadTexture(m_logo);
}

//--------------------------------------------------------------
// Update the state
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void SplashState::Update(float deltaTime)
{
	// Update the timer by the delta time of the application
	m_timer += deltaTime;

	// Go to the menu state after 6 seconds
	if (m_timer > 6.0f)
	{
		m_app->GetGameStateManager()->SetState("Splash", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		m_app->GetGameStateManager()->PushState("Menu");
	}
}

//---------------
// Draw the state
//---------------
void SplashState::Draw()
{
	// For 3 seconds
	if (m_timer < 3.0f)
	{
		// Draw The SEGA logo
		DrawText("Characters owned by", GetScreenWidth() * 0.30, GetScreenHeight() * 0.25, 30, DARKBLUE);
		DrawTexture(m_logo, 0, GetScreenHeight() / 2 - m_logo.height / 2, WHITE);
	}

	// For 3 seconds
	if (m_timer > 3.0f)
	{
		// Tell the player who created the game
		DrawText("Game made by", GetScreenWidth() * 0.37, GetScreenHeight() * 0.25, 30, DARKBLUE);
		DrawText("LOGAN RYAN", GetScreenWidth() * 0.10, GetScreenHeight() * 0.4, 100, DARKBLUE);
	}
}