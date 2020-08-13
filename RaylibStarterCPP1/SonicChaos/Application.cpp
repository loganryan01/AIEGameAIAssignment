#include "Application.h"
#include <functional>

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

	// Chaos Emeralds
	for (int i = 0; i < 7; i++)
	{
		Texture2D texture;
		m_chaosEmeralds[i] = new ChaosEmerald();

		switch (i)
		{
		case 0: 
			texture = LoadTexture("./resources/Chaos Emerald 1.png");
			m_chaosEmeralds[i]->SetPosition({ 40, 24 });
			break;
		case 1: 
			texture = LoadTexture("./resources/Chaos Emerald 2.png"); 
			m_chaosEmeralds[i]->SetPosition({ 40, 760 });
			break;
		case 2: 
			texture = LoadTexture("./resources/Chaos Emerald 3.png"); 
			m_chaosEmeralds[i]->SetPosition({ 136, 376 });
			break;
		case 3: 
			texture = LoadTexture("./resources/Chaos Emerald 4.png"); 
			m_chaosEmeralds[i]->SetPosition({ 392, 344 });
			break;
		case 4: 
			texture = LoadTexture("./resources/Chaos Emerald 5.png"); 
			m_chaosEmeralds[i]->SetPosition({ 648, 376 });
			break;
		case 5: 
			texture = LoadTexture("./resources/Chaos Emerald 6.png"); 
			m_chaosEmeralds[i]->SetPosition({ 744, 24 });
			break;
		case 6: 
			texture = LoadTexture("./resources/Chaos Emerald 7.png"); 
			m_chaosEmeralds[i]->SetPosition({ 744, 760 });
			break;
		}
		m_chaosEmeralds[i]->SetTexture(texture);
	}
	
	// Graph
	m_graph = LoadGraph();
	
	// Agents
	m_player = new PlayerAgent();
	m_player->SetPosition({ 392, 760 });

	// Behaviours
	m_keyboardBehaviour = new KeyboardBehaviour();
	m_seekChaosEmerald = new SeekChaosEmerald();
	m_seekMasterEmerald = new SeekMasterEmerald();
	m_wanderBehaviour = new WanderBehaviour();
	
	m_player->AddBehaviour(m_wanderBehaviour);

	// Camera
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
	m_camera.offset = {(float)m_windowWidth / 2, (float)m_windowHeight / 2};
	m_camera.zoom = 8.0f;

	// Calculate start node:
	for (auto node : m_graph->GetNodes())
	{
		float distance = Vector2Distance(m_player->GetPosition(), node->data);

		if (distance < 16.0f)
		{
			m_startingNode = node;
		}
	}

	m_endNode = m_graph->DepthFirstSearch(m_startingNode, [&](Graph2D::Node* chosenNode) {
		if (chosenNode->doorNode == true)
		{
			return true;
		}
		else
		{
			return false;
		}
		});

	m_myPath = m_graph->AStarSearch(m_startingNode, m_endNode);
	m_myPath.back()->beenHere = true;

	for (auto node : m_myPath)
	{
		m_wanderBehaviour->SetPathNode(node->data);
	}
	
	m_wanderBehaviour->SetTarget(m_myPath.front()->data);
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
	for (int i = 0; i < 7; i++)
	{
		float distance = Vector2Distance(m_chaosEmeralds[i]->GetPosition(), m_player->GetPosition());

		if (distance < 80.0f)
		{
			m_player->RemoveBehaviour();
			m_player->AddBehaviour(m_seekChaosEmerald);
		}
	}
	
	m_player->Update(deltaTime);
	//m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };

	// Switch from wander to seek chaos emerald
	// - Check for a chaos emerald in the room
	// -- Certain distance away from it?
	
	// - Get the position of the chaos emerald

	// Get a new path
	if (m_wanderBehaviour->GetPath().empty())
	{
		for (auto node : m_graph->GetNodes())
		{
			float distance = Vector2Distance(m_player->GetPosition(), node->data);

			if (distance < 16.0f)
			{
				m_startingNode = node;
			}
		}

		m_endNode = m_graph->DepthFirstSearch(m_startingNode, [&](Graph2D::Node* chosenNode) {
			float distance = Vector2Distance(chosenNode->data, m_startingNode->data);
			
			if (chosenNode->doorNode == true && chosenNode->beenHere == false && distance >= 32.0f)
			{
				return true;
			}

			return false;
			});

		m_myPath = m_graph->AStarSearch(m_startingNode, m_endNode);
		m_myPath.back()->beenHere = true;

		for (auto node : m_myPath)
		{
			m_wanderBehaviour->SetPathNode(node->data);
		}

		m_wanderBehaviour->SetTarget(m_myPath.front()->data);
	}
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	//BeginMode2D(m_camera);

	DrawTexture(m_map, 0, 0, WHITE);

	for (int i = 0; i < 7; i++)
	{
		m_chaosEmeralds[i]->Draw();
	}

	m_player->Draw();

	//DrawCircleLines(m_seekChaosEmerald->GetTarget().x, m_seekChaosEmerald->GetTarget().y, 25.0f, BLACK);

	m_graph->Draw();
	m_graph->DrawPath(m_myPath);

	//EndMode2D();

	EndDrawing();
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