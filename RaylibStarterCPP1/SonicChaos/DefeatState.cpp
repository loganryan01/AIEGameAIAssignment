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

	m_playButton = new Button("PLAY", Rectangle{ GetScreenWidth() * 0.1f, GetScreenHeight() * 0.9f, 115, 37 });
	m_playButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Victory", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		m_app->GetGameStateManager()->PushState("Play");
		});

	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.8f, GetScreenHeight() * 0.9f, 110, 37 });
	m_exitButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Victory", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		});
}

void DefeatState::Unload()
{
	delete m_playButton;
	delete m_exitButton;

	UnloadTexture(m_superShadow);
}

void DefeatState::Update(float dt)
{
	m_playButton->Update();
	m_exitButton->Update();
}

void DefeatState::Draw()
{
	DrawTexture(m_superShadow, 0, 0, WHITE);

	DrawText("TRY AGAIN?", GetScreenWidth() * 0.4f, 0, 30, RED);

	m_playButton->Draw();
	m_exitButton->Draw();
}