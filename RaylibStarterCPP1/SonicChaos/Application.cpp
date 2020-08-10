#include "Application.h"

#include "raylib.h"

Application::Application(int windowWidth, int windowHeight) :
	m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
	m_map = { 0, 0, 0, 0, 0 };
	m_camera = { {0,0}, {0,0}, 0, 0 };
}

Application::~Application()
{

}

void Application::Run()
{
	Load();

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		Update(dt);
		Draw();
	}

	delete m_keyboardBehaviour;
	delete m_player;
	UnloadTexture(m_map);

	CloseWindow();
}

void Application::Load()
{
	InitWindow(m_windowWidth, m_windowHeight, "Sonic Chaos");

	// Textures
	m_map = LoadTexture("C:/Users/logan/Desktop/AIEGameAIAssignment/Resources/myTilemap/tilemap.png");

	// Agents
	m_player = new PlayerAgent();

	// Behaviours
	m_keyboardBehaviour = new KeyboardBehaviour();
	m_player->AddBehaviour(m_keyboardBehaviour);

	// Camera
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
	m_camera.offset = {(float)m_windowWidth / 2, (float)m_windowHeight / 2};
	m_camera.zoom = 8.0f;
}

void Application::Update(float deltaTime)
{
	m_player->Update(deltaTime);
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(BLACK);

	BeginMode2D(m_camera);

	DrawTexture(m_map, 0, 0, WHITE);
	m_player->Draw();

	EndMode2D();

	EndDrawing();
}