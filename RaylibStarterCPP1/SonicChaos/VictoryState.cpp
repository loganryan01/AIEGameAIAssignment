#include "VictoryState.h"

#include "Application.h"
#include "GameStateManager.h"

VictoryState::VictoryState(Application* app) : m_app(app)
{
	m_superSonic = { 0 };
}

VictoryState::~VictoryState()
{

}

void VictoryState::Load()
{
	m_superSonic = LoadTexture("./resources/Super Sonic.png");

	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.8f, GetScreenHeight() * 0.9f, 110, 37 });
	m_exitButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Victory", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		});
}

void VictoryState::Unload()
{
	delete m_exitButton;
	
	UnloadTexture(m_superSonic);
}

void VictoryState::Update(float dt)
{
	m_exitButton->Update();
}

void VictoryState::Draw()
{
	DrawTexture(m_superSonic, 0, 0, WHITE);

	DrawText("CONGRATULATIONS!!!!!!", GetScreenWidth() * 0.25f, 0, 30, GOLD);

	m_exitButton->Draw();
}