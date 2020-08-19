/*------------------------------------------------------------
	File Name: MenuState.cpp
	Purpose: Check if the player still wants to play the game.
	Author: Logan Ryan
	Modified: 19 August 2020
--------------------------------------------------------------
	Copyright 2020 Logan Ryan.
------------------------------------------------------------*/

#include "MenuState.h"

#include "Application.h"
#include "GameStateManager.h"

//-----------------------------------------------------
// Constructor
//	app (Application*): Which application are we using?
//-----------------------------------------------------
MenuState::MenuState(Application* app) : m_app(app)
{
	
}

//-----------
// Destructor
//-----------
MenuState::~MenuState()
{

}

//--------------------------------------
// Load the assets needed for this state
//--------------------------------------
void MenuState::Load()
{
	// Create a play button for the player to start playing the game.
	m_playButton = new Button("PLAY", Rectangle{ GetScreenWidth() * 0.45f, GetScreenHeight() * 0.45f, 115, 37 });
	m_playButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Menu", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		m_app->GetGameStateManager()->PushState("Play");
		});

	// Create an exit button for the player to leave the game.
	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.45f, GetScreenHeight() * 0.51f, 110, 37 });
	m_exitButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Menu", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		});
}

//-----------------------------------------------
// Unload the assets that was used for this state
//-----------------------------------------------
void MenuState::Unload()
{
	delete m_playButton;
	delete m_exitButton;
}

//--------------------------------------------------------------
// Update the state
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void MenuState::Update(float dt)
{
	// Update the buttons
	m_playButton->Update();
	m_exitButton->Update();
}

//---------------
// Draw the state
//---------------
void MenuState::Draw()
{
	// Draw the buttons
	m_playButton->Draw();
	m_exitButton->Draw();

	// Draw the name of the game
	DrawText("SONIC CHAOS", GetScreenWidth() * 0.06f, GetScreenHeight() * 0.3f, 100, BLUE);
}