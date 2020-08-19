#include "MenuState.h"

#include "Application.h"
#include "GameStateManager.h"

MenuState::MenuState(Application* app) : m_app(app)
{
	
}

MenuState::~MenuState()
{

}

void MenuState::Load()
{
	m_playButton = new Button("PLAY", Rectangle{ GetScreenWidth() * 0.45f, GetScreenHeight() * 0.45f, 115, 37 });
	m_playButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Menu", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		m_app->GetGameStateManager()->PushState("Play");
		});

	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.45f, GetScreenHeight() * 0.51f, 110, 37 });
	m_exitButton->OnClick([&]() {
		m_app->GetGameStateManager()->SetState("Menu", nullptr); // unload
		m_app->GetGameStateManager()->PopState();
		});
}

void MenuState::Unload()
{
	delete m_playButton;
	delete m_exitButton;
}

void MenuState::Update(float dt)
{
	m_playButton->Update();
	m_exitButton->Update();
}

void MenuState::Draw()
{
	m_playButton->Draw();
	m_exitButton->Draw();

	DrawText("SONIC CHAOS", GetScreenWidth() * 0.06f, GetScreenHeight() * 0.3f, 100, BLUE);
}