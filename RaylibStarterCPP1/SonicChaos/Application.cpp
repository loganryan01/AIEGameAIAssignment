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

	Unload();
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

	// Walls
	Rectangle topWall = { 0, 0, 96, 16 };
	Rectangle leftWall = { 0, 16, 5, 48 };
	Rectangle rightWall = { 91, 0, 5, 64 };
	Rectangle bottomWall = { 0, 64, 43, 16 };
	Rectangle bottomWall1 = { 69, 64, 27, 16 };

	m_topWalls.push_back(topWall);
	m_leftWalls.push_back(leftWall);
	m_rightWalls.push_back(rightWall);
	m_bottomWalls.push_back(bottomWall);
	m_bottomWalls.push_back(bottomWall1);
}

void Application::Unload()
{
	delete m_keyboardBehaviour;
	delete m_player;
	UnloadTexture(m_map);

	CloseWindow();
}

void Application::Update(float deltaTime)
{
	for (int i = 0; i < m_topWalls.size(); i++)
	{
		// Stop player from going up
		if (CheckCollisionRecs(m_player->GetTopAABB(), m_topWalls[i]) && m_player->GetVelocity().y < 0)
		{
			m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
		}
	}

	for (int i = 0; i < m_leftWalls.size(); i++)
	{
		if (CheckCollisionRecs(m_player->GetLeftAABB(), m_leftWalls[i]) && m_player->GetVelocity().x < 0)
		{
			m_player->SetVelocity({ 0, m_player->GetVelocity().y });
		}
	}

	for (int i = 0; i < m_rightWalls.size(); i++)
	{
		if (CheckCollisionRecs(m_player->GetRightAABB(), m_rightWalls[i]) && m_player->GetVelocity().x > 0)
		{
			m_player->SetVelocity({ 0, m_player->GetVelocity().y });
		}
	}

	for (int i = 0; i < m_bottomWalls.size(); i++)
	{
		// Stop player from going up
		if (CheckCollisionRecs(m_player->GetBottomAABB(), m_bottomWalls[i]) && m_player->GetVelocity().y > 0)
		{
			m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
		}
	}
	
	m_player->Update(deltaTime);
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	BeginMode2D(m_camera);

	DrawTexture(m_map, 0, 0, WHITE);
	m_player->Draw();

	for (int i = 0; i < m_topWalls.size(); i++)
	{
		DrawRectangle(m_topWalls[i].x, m_topWalls[i].y, m_topWalls[i].width, m_topWalls[i].height, BLACK);
	}

	for (int i = 0; i < m_leftWalls.size(); i++)
	{
		DrawRectangle(m_leftWalls[i].x, m_leftWalls[i].y, m_leftWalls[i].width, m_leftWalls[i].height, BLACK);
	}

	for (int i = 0; i < m_rightWalls.size(); i++)
	{
		DrawRectangle(m_rightWalls[i].x, m_rightWalls[i].y, m_rightWalls[i].width, m_rightWalls[i].height, BLACK);
	}

	for (int i = 0; i < m_bottomWalls.size(); i++)
	{
		DrawRectangle(m_bottomWalls[i].x, m_bottomWalls[i].y, m_bottomWalls[i].width, m_bottomWalls[i].height, BLACK);
	}

	EndMode2D();

	EndDrawing();
}