/*----------------------------------------------
	File Name: VictoryState.cpp
	Purpose: Tell the player that they have won.
	Author: Logan Ryan
	Modified: 19 August 2020
------------------------------------------------
	Copyright 2020 Logan Ryan.
----------------------------------------------*/

#include "VictoryState.h"

#include "Application.h"
#include "GameStateManager.h"

//-----------------------------------------------------
// Constructor
//	app (Application*): Which application are we using?
//-----------------------------------------------------
VictoryState::VictoryState(Application* app) : m_app(app)
{
	m_superSonic = { 0 };
}

//-----------
// Destructor
//-----------
VictoryState::~VictoryState()
{

}

//--------------------------------------
// Load the assets needed for this state
//--------------------------------------
void VictoryState::Load()
{
	// Load the Super Sonic texture
	m_superSonic = LoadTexture("./resources/Super Sonic.png");

	// Create the exit button for the player to exit the game
	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.8f, GetScreenHeight() * 0.9f, 110, 37 });
	m_exitButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Victory", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		});
}

//-----------------------------------------------
// Unload the assets that was used for this state
//-----------------------------------------------
void VictoryState::Unload()
{
	delete m_exitButton;
	
	UnloadTexture(m_superSonic);
}

//--------------------------------------------------------------
// Update the state
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void VictoryState::Update(float dt)
{
	// Update the exit button
	m_exitButton->Update();
}

//---------------
// Draw the state
//---------------
void VictoryState::Draw()
{
	// Draw Super Sonic
	DrawTexture(m_superSonic, 0, 0, WHITE);

	DrawText("CONGRATULATIONS!!!!!!", GetScreenWidth() * 0.25f, 0, 30, GOLD);

	// Draw exit button
	m_exitButton->Draw();
}