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
		ChaosEmerald* chaosEmerald = new ChaosEmerald();

		switch (i)
		{
		case 0: 
			texture = LoadTexture("./resources/Chaos Emerald 1.png");
			chaosEmerald->SetPosition({ 40, 24 });
			break;
		case 1: 
			texture = LoadTexture("./resources/Chaos Emerald 2.png"); 
			chaosEmerald->SetPosition({ 40, 760 });
			break;
		case 2: 
			texture = LoadTexture("./resources/Chaos Emerald 3.png"); 
			chaosEmerald->SetPosition({ 136, 376 });
			break;
		case 3: 
			texture = LoadTexture("./resources/Chaos Emerald 4.png"); 
			chaosEmerald->SetPosition({ 392, 344 });
			break;
		case 4: 
			texture = LoadTexture("./resources/Chaos Emerald 5.png"); 
			chaosEmerald->SetPosition({ 648, 376 });
			break;
		case 5: 
			texture = LoadTexture("./resources/Chaos Emerald 6.png"); 
			chaosEmerald->SetPosition({ 744, 24 });
			break;
		case 6: 
			texture = LoadTexture("./resources/Chaos Emerald 7.png"); 
			chaosEmerald->SetPosition({ 744, 760 });
			break;
		}
		chaosEmerald->SetTexture(texture);
		m_chaosEmeralds.push_back(chaosEmerald);
	}

	// Master Emerald
	m_masterEmerald = new MasterEmerald();
	m_masterEmerald->SetTexture(LoadTexture("./resources/Master Emerald.png"));
	m_masterEmerald->SetPosition({ 384 , 16 });
	
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
	
	m_player->SetBehaviour(m_wanderBehaviour);

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
	float distanceToEndOfPath = 0.0f;
	
	// Check if we are at the end of the path
	if (!m_myPath.empty())
	{
		distanceToEndOfPath = Vector2Distance(m_player->GetPosition(), m_myPath.back()->data);
	}

	// Check if we have a path
	if (m_myPath.empty() || distanceToEndOfPath < 8.0f)
	{
		std::cout << "Calculating new path to ";
		m_myPath.clear();

		if (m_player->GetBehaviour() == m_wanderBehaviour)
		{
			std::cout << "Door node" << std::endl;
			m_myPath = PathToDoorNode();

			// Switch from wander to seek chaos emerald if the distance from the player to the chaos node is less than 48.0f
			for (auto node : m_graph->GetNodes())
			{
				float distanceToChaosNode = Vector2Distance(m_player->GetPosition(), node->data);

				if (node->chaosNode == true && node->beenHere == false && distanceToChaosNode < 80.0f && m_player->GetBehaviour() == m_wanderBehaviour)
				{
					m_player->SetBehaviour(m_seekChaosEmerald);
					std::cout << "Changed to seek chaos emerald" << std::endl;
					m_myPath.clear();
					break;
				}
			}
		}
		else if (m_player->GetBehaviour() == m_seekChaosEmerald)
		{
			std::cout << "Chaos node" << std::endl;
			m_myPath = PathToChaosNode();

			// Switch from seek chaos emerald to wander once the chaos emerald is collected
			if (m_player->GetBehaviour() == m_seekChaosEmerald)
			{
				// Remove chaos emerald
				for (int i = 0; i < m_chaosEmeralds.size(); i++)
				{
					float distanceToChaosEmerald = Vector2Distance(m_player->GetPosition(), m_chaosEmeralds[i]->GetPosition());
					std::cout << distanceToChaosEmerald << std::endl;

					if (distanceToChaosEmerald < 64.0f)
					{
						std::cout << "Got emerald" << std::endl;
						m_myPath.back()->beenHere = true;
						m_chaosEmeralds.erase(m_chaosEmeralds.begin() + i);
						m_player->SetBehaviour(m_wanderBehaviour);
						std::cout << "Changed to wander" << std::endl;
						m_seekChaosEmerald->GetPath().clear();
						m_seekChaosEmerald->SetTarget({ NULL, NULL });
						break;
					}
				}
			}
		}
	}
	
	m_player->Update(deltaTime);

	//m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	//BeginMode2D(m_camera);

	DrawTexture(m_map, 0, 0, WHITE);

	for (int i = 0; i < m_chaosEmeralds.size(); i++)
	{
		m_chaosEmeralds[i]->Draw();
	}

	m_masterEmerald->Draw();

	m_player->Draw();

	if (!m_myPath.empty())
	{
		DrawCircleLines(m_myPath.back()->data.x, m_myPath.back()->data.y, 16.0f, BLACK);
	}

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

			// Get pixel color from texture c.b == 144
			auto c = pixels[y * m_image.width + x];
			if (c.a > 0 && c.b == 144)
			{
				graph->SetDoorNode(xPos, yPos);
				
			}
			else if (c.a > 0 && c.b == 153)
			{
				graph->SetChaosNode(xPos, yPos);
				
			}
			else if (c.a > 0 && c.b == 236)
			{
				graph->SetMasterNode(xPos, yPos);
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

std::list<Graph2D::Node*> Application::PathToDoorNode()
{
	Graph2D::Node* startingNode = nullptr;
	Graph2D::Node* endNode = nullptr;
	std::list<Graph2D::Node*> doorPath;

	// Get Starting node
	for (auto node : m_graph->GetNodes())
	{
		float distanceToStartNode = Vector2Distance(m_player->GetPosition(), node->data);

		if (distanceToStartNode < 16.0f)
		{
			startingNode = node;
			break;
		}
	}

	// End node should be a door node that we haven't visited yet
	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {
		float distance = Vector2Distance(startingNode->data, chosenNode->data);

		if (chosenNode->doorNode == true && chosenNode->beenHere == false && distance >= 32.0f)
		{
			return true;
		}

		return false;
		});

	doorPath = m_graph->dijkstrasSearch(startingNode, endNode);
	doorPath.back()->beenHere = true;

	for (auto node : doorPath)
	{
		m_wanderBehaviour->SetPathNode(node->data);
	}

	m_wanderBehaviour->SetTarget(doorPath.front()->data);

	return doorPath;
}

std::list<Graph2D::Node*> Application::PathToChaosNode()
{
	Graph2D::Node* startingNode = nullptr;
	Graph2D::Node* endNode = nullptr;
	std::list<Graph2D::Node*> chaosPath;
	
	for (auto node : m_graph->GetNodes())
	{
		float distanceToStartNode = Vector2Distance(m_player->GetPosition(), node->data);

		if (distanceToStartNode < 16.0f)
		{
			startingNode = node;
			break;
		}
	}

	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {
		float distanceToChosenNode = Vector2Distance(startingNode->data, chosenNode->data);

		// If the chosen node is a chaos node, we haven't been to that node, and we are close to it
		if (chosenNode->chaosNode == true && chosenNode->beenHere == false && distanceToChosenNode < 80.0f)
		{
			std::cout << "Chaos Node has been found" << std::endl;
			return true;
		}

		return false;
		});

	chaosPath = m_graph->dijkstrasSearch(startingNode, endNode);
	chaosPath.back()->beenHere = true;

	for (auto node : chaosPath)
	{
		m_seekChaosEmerald->SetPathNode(node->data);
	}

	m_seekChaosEmerald->SetTarget(chaosPath.front()->data);

	return chaosPath;
}