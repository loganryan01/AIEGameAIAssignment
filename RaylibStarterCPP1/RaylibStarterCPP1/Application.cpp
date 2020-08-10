#include "Application.h"

#include "raylib.h"

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

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		Update(dt);
		Draw();
	}

	CloseWindow();
}

void Application::Update(float deltaTime)
{
	
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	EndDrawing();
}