#include "DefeatState.h"

#include "Application.h"
#include "GameStateManager.h"

DefeatState::DefeatState(Application* app) : m_app(app)
{
	m_superShadow = { 0 };
}

DefeatState::~DefeatState()
{

}

void DefeatState::Load()
{
	m_superShadow = LoadTexture("./resources/Super Shadow.png");

	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.8f, GetScreenHeight() * 0.9f, 110, 37 });
	m_exitButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Defeat", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		});
}

void DefeatState::Unload()
{
	delete m_exitButton;

	UnloadTexture(m_superShadow);
}

void DefeatState::Update(float dt)
{
	m_exitButton->Update();
}

void DefeatState::Draw()
{
	DrawTexture(m_superShadow, 0, 0, WHITE);

	DrawText("BETTER LUCK NEXT TIME", GetScreenWidth() * 0.3f, 0, 30, RED);

	m_exitButton->Draw();
}