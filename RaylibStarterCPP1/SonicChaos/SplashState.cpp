#include "SplashState.h"

#include "Application.h"
#include "GameStateManager.h"

SplashState::SplashState(Application* app) : m_app(app)
{
	m_logo = { 0 };
}

SplashState::~SplashState()
{

}

void SplashState::Load()
{
	m_logo = LoadTexture("./resources/SEGA logo.png");
}

void SplashState::Unload()
{
	UnloadTexture(m_logo);
}

void SplashState::Update(float deltaTime)
{
	m_timer += deltaTime;
}

void SplashState::Draw()
{
	if (m_timer < 3.0f)
	{
		DrawText("Characters owned by", GetScreenWidth() * 0.30, GetScreenHeight() * 0.25, 30, DARKBLUE);
		DrawTexture(m_logo, 0, GetScreenHeight() / 2 - m_logo.height / 2, WHITE);
	}

	if (m_timer > 3.0f)
	{
		DrawText("Game made by", GetScreenWidth() * 0.37, GetScreenHeight() * 0.25, 30, DARKBLUE);
		DrawText("LOGAN RYAN", GetScreenWidth() * 0.10, GetScreenHeight() * 0.4, 100, DARKBLUE);
	}
}