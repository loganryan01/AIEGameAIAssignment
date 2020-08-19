#include "MenuState.h"

#include <iostream>

MenuState::MenuState(Application* app) : m_app(app)
{
	
}

MenuState::~MenuState()
{

}

void MenuState::Load()
{
	m_playButton = new Button("PLAY", Rectangle{ GetScreenWidth() * 0.45f, GetScreenHeight() * 0.45f, 115, 37 });
	m_playButton->OnClick([]() {
		std::cout << "Play button has been clicked" << std::endl; 
		});

	m_exitButton = new Button("EXIT", Rectangle{ GetScreenWidth() * 0.45f, GetScreenHeight() * 0.51f, 110, 37 });
	m_exitButton->OnClick([]() {
		std::cout << "Exit button has been clicked" << std::endl;
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