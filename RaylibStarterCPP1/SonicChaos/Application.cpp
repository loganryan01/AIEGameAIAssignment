#include "Application.h"

#include "raylib.h"

Application::Application(int windowWidth, int windowHeight) :
	m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
	m_map = { 0, 0, 0, 0, 0 };
	m_image = { 0, 0, 0, 0, 0 };
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
	m_image = LoadImage("./resources/tilemap.png");
	m_map = LoadTextureFromImage(m_image);
	m_graph = LoadGraph();
	
	// Agents
	m_player = new PlayerAgent();
	m_player->SetPosition({ 360, 330 });

	// Behaviours
	m_keyboardBehaviour = new KeyboardBehaviour();
	m_seekChaosEmerald = new SeekChaosEmerald();
	m_seekMasterEmerald = new SeekMasterEmerald();
	
	m_seekMasterEmerald->SetTarget({ 240,40 });
	m_player->AddBehaviour(m_seekChaosEmerald);

	// Camera
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
	m_camera.offset = {(float)m_windowWidth / 2, (float)m_windowHeight / 2};
	m_camera.zoom = 8.0f;

	m_myPath = m_graph->dijkstrasSearch(m_graph->GetNodes().front(), m_graph->GetNodes().back());
	m_seekChaosEmerald->SetTarget(m_graph->GetNodes()[0]->data);
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
	//for (int i = 0; i < m_topWalls.size(); i++)
	//{
	//	// Stop player from going up
	//	if (CheckCollisionRecs(m_player->GetTopAABB(), m_topWalls[i]) && m_player->GetVelocity().y < 0)
	//	{
	//		m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
	//	}
	//}

	//for (int i = 0; i < m_leftWalls.size(); i++)
	//{
	//	if (CheckCollisionRecs(m_player->GetLeftAABB(), m_leftWalls[i]) && m_player->GetVelocity().x < 0)
	//	{
	//		m_player->SetVelocity({ 0, m_player->GetVelocity().y });
	//	}
	//}

	//for (int i = 0; i < m_rightWalls.size(); i++)
	//{
	//	if (CheckCollisionRecs(m_player->GetRightAABB(), m_rightWalls[i]) && m_player->GetVelocity().x > 0)
	//	{
	//		m_player->SetVelocity({ 0, m_player->GetVelocity().y });
	//	}
	//}

	//for (int i = 0; i < m_bottomWalls.size(); i++)
	//{
	//	// Stop player from going up
	//	if (CheckCollisionRecs(m_player->GetBottomAABB(), m_bottomWalls[i]) && m_player->GetVelocity().y > 0)
	//	{
	//		m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
	//	}
	//}
	
	m_player->Update(deltaTime);
	//m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	//BeginMode2D(m_camera);

	DrawTexture(m_map, 0, 0, WHITE);
	m_player->Draw();

	/*for (int i = 0; i < m_topWalls.size(); i++)
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
	}*/

	//DrawCircleLines(m_seekChaosEmerald->GetTarget().x, m_seekChaosEmerald->GetTarget().y, 25.0f, BLACK);

	m_graph->Draw();
	m_graph->DrawPath(m_myPath);

	//EndMode2D();

	EndDrawing();
}

void Application::CreateTopWall(float x, float y, float width, float height)
{
	Rectangle topWall = { x, y, width, height };
	m_topWalls.push_back(topWall);
}

void Application::CreateLeftWall(float x, float y, float width, float height)
{
	Rectangle leftWall = { x, y, width, height };
	m_leftWalls.push_back(leftWall);
}

void Application::CreateRightWall(float x, float y, float width, float height)
{
	Rectangle rightWall = { x, y, width, height };
	m_rightWalls.push_back(rightWall);
}

void Application::CreateBottomWall(float x, float y, float width, float height)
{
	Rectangle bottomWall = { x, y, width, height };
	m_bottomWalls.push_back(bottomWall);
}

void Application::LoadLeftWall()
{
	CreateLeftWall(0, 16, 5, 48);
	CreateLeftWall(0, 144, 5, 48);
	CreateLeftWall(0, 240, 5, 96);
	CreateLeftWall(0, 704, 5, 96);
	CreateLeftWall(27, 64, 5, 64);
	CreateLeftWall(27, 192, 5, 64);
	CreateLeftWall(96, 32, 5, 96);
	CreateLeftWall(96, 336, 5, 80);
	CreateLeftWall(96, 704, 5, 16);
	CreateLeftWall(96, 752, 5, 48);
	CreateLeftWall(123, 112, 5, 32);
	CreateLeftWall(123, 320, 5, 32);
	CreateLeftWall(128, 576, 5, 96);
	CreateLeftWall(192, 0, 5, 64);
	CreateLeftWall(192, 96, 5, 48);
	CreateLeftWall(192, 176, 5, 32);
	CreateLeftWall(192, 240, 5, 32);
	CreateLeftWall(192, 304, 5, 32);
	CreateLeftWall(192, 400, 5, 96);
	CreateLeftWall(192, 512, 5, 96);
	CreateLeftWall(192, 640, 5, 48);
	CreateLeftWall(219, 192, 5, 64);
	CreateLeftWall(219, 320, 5, 96);
	CreateLeftWall(219, 560, 5, 32);
	CreateLeftWall(219, 672, 5, 48);
	CreateLeftWall(288, 0, 5, 64);
	CreateLeftWall(288, 96, 5, 48);
	CreateLeftWall(288, 176, 5, 32);
	CreateLeftWall(288, 240, 5, 32);
	CreateLeftWall(288, 592, 5, 16);
	CreateLeftWall(352, 320, 5, 112);
	CreateLeftWall(352, 464, 5, 32);
	CreateLeftWall(352, 720, 5, 80);
	CreateLeftWall(379, 384, 5, 32);
	CreateLeftWall(379, 480, 5, 128);
	CreateLeftWall(379, 640, 5, 96);
	CreateLeftWall(416, 0, 5, 208);
	CreateLeftWall(448, 400, 5, 32);
	CreateLeftWall(448, 464, 5, 32);
	CreateLeftWall(512, 0, 5, 64);
	CreateLeftWall(512, 96, 5, 48);
	CreateLeftWall(512, 176, 5, 32);
	CreateLeftWall(512, 240, 5, 32);
	CreateLeftWall(512, 304, 5, 32);
	CreateLeftWall(512, 512, 5, 96);
	CreateLeftWall(512, 640, 5, 48);
	CreateLeftWall(539, 192, 5, 64);
	CreateLeftWall(539, 320, 5, 96);
	CreateLeftWall(539, 560, 5, 32);
	CreateLeftWall(539, 672, 5, 48);
	CreateLeftWall(608, 32, 5, 32);
	CreateLeftWall(608, 96, 5, 48);
	CreateLeftWall(608, 176, 5, 32);
	CreateLeftWall(608, 240, 5, 32);
	CreateLeftWall(608, 304, 5, 112);
	CreateLeftWall(608, 576, 5, 32);
	CreateLeftWall(608, 640, 5, 32);
	CreateLeftWall(635, 112, 5, 32);
	CreateLeftWall(635, 320, 5, 32);
	CreateLeftWall(704, 0, 5, 80);
	CreateLeftWall(704, 704, 5, 16);
	CreateLeftWall(704, 752, 5, 48);
	CreateLeftWall(731, 64, 5, 80);
	CreateLeftWall(731, 192, 5, 64);
}

void Application::LoadRightWall()
{
	CreateRightWall(64, 64, 5, 80);
	CreateRightWall(64, 192, 5, 64);
	CreateRightWall(91, 0, 5, 80);
	CreateRightWall(91, 704, 5, 16);
	CreateRightWall(91, 752, 5, 48);
	CreateRightWall(160, 112, 5, 32);
	CreateRightWall(160, 320, 5, 32);
	CreateRightWall(187, 32, 5, 32);
	CreateRightWall(187, 96, 5, 48);
	CreateRightWall(187, 176, 5, 32);
	CreateRightWall(187, 240, 5, 32);
	CreateRightWall(187, 304, 5, 112);
	CreateRightWall(187, 576, 5, 32);
	CreateRightWall(187, 640, 5, 32);
	CreateRightWall(256, 192, 5, 64);
	CreateRightWall(256, 320, 5, 96);
	CreateRightWall(256, 560, 5, 32);
	CreateRightWall(256, 672, 5, 48);
	CreateRightWall(283, 0, 5, 64);
	CreateRightWall(283, 96, 5, 48);
	CreateRightWall(283, 176, 5, 32);
	CreateRightWall(283, 240, 5, 32);
	CreateRightWall(283, 304, 5, 32);
	CreateRightWall(283, 512, 5, 96);
	CreateRightWall(283, 640, 5, 48);
	CreateRightWall(347, 400, 5, 32);
	CreateRightWall(347, 464, 5, 32);
	CreateRightWall(379, 0, 5, 208);
	CreateRightWall(416, 384, 5, 32);
	CreateRightWall(416, 480, 5, 128);
	CreateRightWall(416, 640, 5, 96);
	CreateRightWall(443, 320, 5, 112);
	CreateRightWall(443, 464, 5, 32);
	CreateRightWall(443, 720, 5, 80);
	CreateRightWall(507, 0, 5, 64);
	CreateRightWall(507, 96, 5, 48);
	CreateRightWall(507, 176, 5, 32);
	CreateRightWall(507, 240, 5, 32);
	CreateRightWall(507, 592, 5, 16);
	CreateRightWall(576, 192, 5, 64);
	CreateRightWall(576, 320, 5, 96);
	CreateRightWall(576, 560, 5, 32);
	CreateRightWall(576, 672, 5, 48);
	CreateRightWall(603, 0, 5, 64);
	CreateRightWall(603, 96, 5, 48);
	CreateRightWall(603, 176, 5, 32);
	CreateRightWall(603, 240, 5, 32);
	CreateRightWall(603, 304, 5, 32);
	CreateRightWall(603, 400, 5, 96);
	CreateRightWall(603, 512, 5, 96);
	CreateRightWall(603, 640, 5, 48);
	CreateRightWall(667, 576, 5, 96);
	CreateRightWall(672, 112, 5, 32);
	CreateRightWall(672, 320, 5, 32);
	CreateRightWall(704, 32, 5, 96);
	CreateRightWall(704, 336, 5, 80);
	CreateRightWall(704, 704, 5, 16);
	CreateRightWall(704, 752, 5, 48);
	CreateRightWall(768, 64, 5, 64);
	CreateRightWall(768, 192, 5, 64);
	CreateRightWall(795, 16, 5, 48);
	CreateRightWall(795, 144, 5, 48);
	CreateRightWall(795, 240, 5, 96);
	CreateRightWall(795, 704, 5, 96);
}

Graph2D* Application::LoadGraph()
{
	Image image = LoadImage("./resources/graph.png");
	Color* pixels = GetImageData(image);
	Graph2D* graph = new Graph2D;
	std::vector<Graph2D::Node*> nodes;

	int tileWidth = 16;
	int tileHeight = 16;

	for (int y = 0; y < m_image.height; y += tileHeight)
	{
		for (int x = 0; x < m_image.width; x += tileWidth)
		{
 			float xPos = x + 8;
			float yPos = y + 8;

			// Get pixel color from texture
			auto c = pixels[y * m_image.width + x];
			if (c.a > 0 && c.b < 210)
			{
				graph->SetDoorNode(xPos, yPos);
				
			}
			else if (c.a > 0)
			{
				graph->AddNode({ xPos, yPos });
				
			}
		}
	}

	for (auto node : graph->GetNodes())
	{
		graph->SetEdges(node, 32.0f);
	}

  	return graph;
}