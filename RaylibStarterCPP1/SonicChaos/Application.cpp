#include "Application.h"
#include "GameStateManager.h"
#include "IGameState.h"
#include "PlayState.h"
#include "SplashState.h"
#include "MenuState.h"

#include "raylib.h"

// TODO:
// - Game States
// - EXTRA: Health and health packs

Application::Application(int windowWidth, int windowHeight) :
	m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{

}

Application::~Application()
{

}

void Application::Run()
{
	InitWindow(m_windowWidth, m_windowHeight, "Sonic Chaos");

	m_gameStateManager = new GameStateManager();
	
	//m_gameStateManager->SetState("Splash", new SplashState(this));
	m_gameStateManager->SetState("Menu", new MenuState(this));
	//m_gameStateManager->SetState("Play", new PlayState(this));
	
	//m_gameStateManager->PushState("Splash");
	m_gameStateManager->PushState("Menu");
	//m_gameStateManager->PushState("Play");

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		Update(dt);
		Draw();
	}

	delete m_gameStateManager;

	CloseWindow();
}

void Application::Update(float deltaTime)
{
	m_gameStateManager->Update(deltaTime);
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	m_gameStateManager->Draw();

	EndDrawing();
}