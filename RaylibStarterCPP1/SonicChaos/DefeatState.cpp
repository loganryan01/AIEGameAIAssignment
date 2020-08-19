/*-----------------------------------------------
	File Name: DefeatState.cpp
	Purpose: Tell the player that they have lost.
	Author: Logan Ryan
	Modified: 19 August 2020
-------------------------------------------------
	Copyright 2020 Logan Ryan.
-----------------------------------------------*/

#include "DefeatState.h"

#include "Application.h"
#include "GameStateManager.h"

//-----------------------------------------------------
// Constructor
//	app (Application*): Which application are we using?
//-----------------------------------------------------
DefeatState::DefeatState(Application* app) : m_app(app)
{
	m_superShadow = { 0 };
}

//-----------
// Destructor
//-----------
DefeatState::~DefeatState()
{

}

//--------------------------------------
// Load the assets needed for this state
//--------------------------------------
void DefeatState::Load()
{
	// Load the Super Shadow texture
	m_superShadow = LoadTexture("./resources/Super Shadow.png");

	// Create the exit button for the player to exit the game
	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.8f, GetScreenHeight() * 0.9f, 110, 37 });
	m_exitButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Defeat", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		});
}

//-----------------------------------------------
// Unload the assets that was used for this state
//-----------------------------------------------
void DefeatState::Unload()
{
	delete m_exitButton;

	UnloadTexture(m_superShadow);
}

//--------------------------------------------------------------
// Update the state
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void DefeatState::Update(float dt)
{
	// Update the exit button
	m_exitButton->Update();
}

//---------------
// Draw the state
//---------------
void DefeatState::Draw()
{
	// Draw Super Shadow
	DrawTexture(m_superShadow, 0, 0, WHITE);

	DrawText("BETTER LUCK NEXT TIME", GetScreenWidth() * 0.3f, 0, 30, RED);

	// Draw exit button
	m_exitButton->Draw();
}