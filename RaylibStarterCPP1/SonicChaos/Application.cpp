#include "Application.h"
#include <functional>

#include "raylib.h"

// TODO:
// - Stun behaviour
// - Fix flee and attack behaviour
// - Game States
// - EXTRA: Health and health packs

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
	m_player->SetPosition({ 424, 760 });

	m_enemy = new EnemyAgent();
	m_enemy->SetPosition({ 392, 760 });

	// Behaviours
	m_keyboardBehaviour = new KeyboardBehaviour();
	m_seekChaosEmerald = new SeekChaosEmerald();
	m_seekMasterEmerald = new SeekMasterEmerald();
	m_wanderBehaviour = new WanderBehaviour();
	m_attackBehaviour = new AttackBehaviour();
	m_fleeBehaviour = new FleeBehaviour();
	
	m_player->SetBehaviour(m_keyboardBehaviour);
	m_enemy->SetBehaviour(m_wanderBehaviour);

	// Camera
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
	m_camera.offset = {(float)m_windowWidth / 2, (float)m_windowHeight / 2};
	m_camera.zoom = 5.0f;

	// Calculate start node:
	for (auto node : m_graph->GetNodes())
	{
		float distance = Vector2Distance(m_enemy->GetPosition(), node->data);

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
	delete m_seekChaosEmerald;
	delete m_seekMasterEmerald;
	delete m_wanderBehaviour;
	delete m_attackBehaviour;
	delete m_fleeBehaviour;

	delete m_player;
	delete m_enemy;
	UnloadTexture(m_map);

	CloseWindow();
}

void Application::Update(float deltaTime)
{
	float distanceToEndOfPath = 0.0f;
	
	// Check if we are at the end of the path
	if (!m_myPath.empty())
	{
		distanceToEndOfPath = Vector2Distance(m_enemy->GetPosition(), m_myPath.back()->data);
	}

	//----- Behaviour switching -----
	if (distanceToEndOfPath < 12.0f)
	{
		if (m_enemy->GetBehaviour() == m_wanderBehaviour)
		{
			// Switch from wander to seek chaos emerald if the distance from the player to the chaos node is less than 48.0f
			for (auto node : m_graph->GetNodes())
			{
				float distanceToChaosNode = Vector2Distance(m_enemy->GetPosition(), node->data);

				if (node->chaosNode == true && node->beenHere == false && distanceToChaosNode < 78.0f)
				{
					m_enemy->SetBehaviour(m_seekChaosEmerald);
					break;
				}
			}

			float distanceToPlayer = Vector2Distance(m_enemy->GetPosition(), m_player->GetPosition());

			if (m_enemyScore == 7)
			{
				m_enemy->SetBehaviour(m_seekMasterEmerald);
			}
			else if (m_enemyScore + m_playerScore == 7 && distanceToPlayer < 80.0f && m_enemy->GetAttackCharger() == 600.0f)
			{
				// No more chaos emeralds, nobody has 7, enemy can see the player, attack is ready
				m_enemy->SetBehaviour(m_attackBehaviour);
			}
		}
		else if (m_enemy->GetBehaviour() == m_seekChaosEmerald)
		{
			// Remove chaos emerald if there is one 
			for (int i = 0; i < m_chaosEmeralds.size(); i++)
			{
				float distanceToChaosEmerald = Vector2Distance(m_enemy->GetPosition(), m_chaosEmeralds[i]->GetPosition());

				if (distanceToChaosEmerald < 44.0f)
				{
					m_chaosEmeralds.erase(m_chaosEmeralds.begin() + i);
					m_enemyScore++;
					m_enemy->SetBehaviour(m_wanderBehaviour);
					break;
				}
				else if (i == m_chaosEmeralds.size() - 1)
				{
					m_enemy->SetBehaviour(m_wanderBehaviour);
					break;
				}
			}

			if (m_enemyScore == 7)
			{
				m_enemy->SetBehaviour(m_seekMasterEmerald);
			}
		}
		else if (m_enemy->GetBehaviour() == m_seekMasterEmerald)
		{
			if (m_enemyScore < 7)
			{
				m_enemy->SetBehaviour(m_wanderBehaviour);
			}
		}
	}

	if (m_enemy->GetBehaviour() == m_attackBehaviour)
	{
		// Center of the player texture
		float playerXPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
		float playerYPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

		// Center of the enemy texture
		float enemyXPos = m_enemy->GetPosition().x + m_enemy->GetWidth() / 2.0f;
		float enemyYPos = m_enemy->GetPosition().y + m_enemy->GetHeight() / 2.0f;

		float distanceToPlayer = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

		if (distanceToPlayer < m_enemy->GetAttackRadius())
		{
			m_enemy->ResetAttackCharger();
		}

		// Switch to flee once we have attacked
		if (m_enemy->GetAttackCharger() < 600.0f)
		{
			m_enemy->SetBehaviour(m_fleeBehaviour);
		}
		else if (distanceToPlayer > 80.0f)
		{
			m_enemy->SetBehaviour(m_wanderBehaviour);
		}
	}
	else if (m_enemy->GetBehaviour() == m_fleeBehaviour)
	{
		// Center of the player texture
		float playerXPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
		float playerYPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

		// Center of the enemy texture
		float enemyXPos = m_enemy->GetPosition().x + m_enemy->GetWidth() / 2.0f;
		float enemyYPos = m_enemy->GetPosition().y + m_enemy->GetHeight() / 2.0f;

		float distanceFromPlayer = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

		if (distanceFromPlayer > 80.0f)
		{
			m_enemy->SetBehaviour(m_wanderBehaviour);
		}
	}

	//----- Pathfinding -----
	// Check if we have a path or at the end of the path
	if (m_myPath.empty() || distanceToEndOfPath < 12.0f)
	{
		m_myPath.clear();

		if (m_enemy->GetBehaviour() == m_wanderBehaviour)
		{
			m_myPath = PathToDoorNode();
		}
		else if (m_enemy->GetBehaviour() == m_seekChaosEmerald)
		{
			m_myPath = PathToChaosNode();
		}
		else if (m_enemy->GetBehaviour() == m_seekMasterEmerald)
		{
			m_myPath = PathToMasterNode();
		}
		else if (m_enemy->GetBehaviour() == m_attackBehaviour)
		{
			m_myPath = PathToAttackPlayer();

			if (m_myPath.empty())
			{
				// Center of the player texture
				float playerXPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
				float playerYPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

				// Center of the enemy texture
				float enemyXPos = m_enemy->GetPosition().x + m_enemy->GetWidth() / 2.0f;
				float enemyYPos = m_enemy->GetPosition().y + m_enemy->GetHeight() / 2.0f;

				float distanceFromPlayer = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

				if (distanceFromPlayer < 80.0f)
				{
					m_attackBehaviour->SetTarget({ playerXPos, playerYPos });
				}
			}
		}
		else if (m_enemy->GetBehaviour() == m_fleeBehaviour)
		{
			m_myPath = PathToAvoidPlayer();
		}
	}

	//if (m_enemy->GetBehaviour() == m_attackBehaviour)
	//{
	//	// Center of the player texture
	//	float xPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
	//	float yPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

	//	m_attackBehaviour->SetTarget({ xPos, yPos });
	//}
	//else if (m_enemy->GetBehaviour() == m_fleeBehaviour)
	//{
	//	// Center of the player texture
	//	float xPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
	//	float yPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

	//	m_fleeBehaviour->SetTarget({ xPos, yPos });
	//}

	//----- Collision -----
	for (auto barricade : m_graph->GetBarricades())
	{
		if (CheckCollisionRecs(m_player->GetTopAABB(), barricade) && m_player->GetVelocity().y < 0)
		{
			m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
		}

		if (CheckCollisionRecs(m_player->GetBottomAABB(), barricade) && m_player->GetVelocity().y > 0)
		{
			m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
		}
		
		if (CheckCollisionRecs(m_player->GetLeftAABB(), barricade) && m_player->GetVelocity().x < 0)
		{
			m_player->SetVelocity({ 0, m_player->GetVelocity().y });
		}

		if (CheckCollisionRecs(m_player->GetRightAABB(), barricade) && m_player->GetVelocity().x > 0)
		{
			m_player->SetVelocity({ 0, m_player->GetVelocity().y });
		}

		if (CheckCollisionRecs(m_enemy->GetTopAABB(), barricade) && m_enemy->GetVelocity().y < 0)
		{
			m_enemy->SetVelocity({ m_enemy->GetVelocity().x, 0 });
		}

		if (CheckCollisionRecs(m_enemy->GetBottomAABB(), barricade) && m_enemy->GetVelocity().y > 0)
		{
			m_enemy->SetVelocity({ m_enemy->GetVelocity().x, 0 });
		}

		if (CheckCollisionRecs(m_enemy->GetLeftAABB(), barricade) && m_enemy->GetVelocity().x < 0)
		{
			m_enemy->SetVelocity({ 0, m_enemy->GetVelocity().y });
		}

		if (CheckCollisionRecs(m_enemy->GetRightAABB(), barricade) && m_enemy->GetVelocity().x > 0)
		{
			m_enemy->SetVelocity({ 0, m_enemy->GetVelocity().y });
		}
	}
	
	//----- Attack -----
	// Charge the attack bar
	if (m_player->GetAttackCharger() < 600.0f)
	{
		m_player->IncrementAttackCharger();
	}

	if (m_enemy->GetAttackCharger() < 600.0f)
	{
		m_enemy->IncrementAttackCharger();
	}

	// Remove chaos emerald if there is one 
	for (int i = 0; i < m_chaosEmeralds.size(); i++)
	{
		float distanceToChaosEmerald = Vector2Distance(m_player->GetPosition(), m_chaosEmeralds[i]->GetPosition());

		if (distanceToChaosEmerald < 44.0f)
		{
			for (auto node : m_graph->GetNodes())
			{
				if (node->chaosNode == true)
				{
					float distanceToChaosNode = Vector2Distance(m_player->GetPosition(), node->data);

					if (distanceToChaosNode < 66.0f)
					{
						node->chaosNode = false;
					}
				}
			}
			
			m_chaosEmeralds.erase(m_chaosEmeralds.begin() + i);
			m_playerScore++;
			break;
		}
	}
	
	m_player->Update(deltaTime);
	m_enemy->Update(deltaTime);

	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
}

void Application::Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	//BeginMode2D(m_camera);

	DrawTexture(m_map, 0, 0, WHITE);

	if (m_enemy->GetAttackCharger() == 600.0f &&
		CheckCollisionCircleRec({ m_enemy->GetPosition().x + (float)m_enemy->GetWidth() / 2.0f, m_enemy->GetPosition().y + (float)m_enemy->GetHeight() / 2.0f },
			m_enemy->GetAttackRadius(), { m_player->GetPosition().x, m_player->GetPosition().y, (float)m_player->GetWidth(), (float)m_player->GetHeight() }) &&
		m_enemy->GetBehaviour() == m_attackBehaviour)
	{
		DrawCircle(m_enemy->GetPosition().x + m_enemy->GetWidth() / 2, m_enemy->GetPosition().y + m_enemy->GetHeight() / 2, m_enemy->GetAttackRadius(), PURPLE);
	}

	for (int i = 0; i < m_chaosEmeralds.size(); i++)
	{
		m_chaosEmeralds[i]->Draw();
	}

	m_masterEmerald->Draw();

	m_player->Draw();
	m_enemy->Draw();

	

	m_graph->Draw();
	m_graph->DrawPath(m_myPath);

	//EndMode2D();

	if (m_player->GetAttackCharger() == 600.0f)
	{
		DrawText("Attack is Ready", 10, 10, 14, BLACK);
	}

	if (m_enemy->GetBehaviour() == m_wanderBehaviour)
	{
		DrawText("Enemy is wandering", 10, 30, 14, BLACK);
	}
	else if (m_enemy->GetBehaviour() == m_seekChaosEmerald)
	{
		DrawText("Enemy is seeking chaos emerald", 10, 30, 14, BLACK);
	}
	else if (m_enemy->GetBehaviour() == m_seekMasterEmerald)
	{
		DrawText("Enemy is seeking master emerald", 10, 30, 14, BLACK);
	}
	else if (m_enemy->GetBehaviour() == m_attackBehaviour)
	{
		DrawText("Enemy is attacking", 10, 30, 14, BLACK);
	}
	else if (m_enemy->GetBehaviour() == m_fleeBehaviour)
	{
		DrawText("Enemy is fleeing", 10, 30, 14, BLACK);
	}

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
			else if (c.a > 0 && c.b == 0)
			{
				graph->AddBarricade(xPos, yPos);
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
		float distanceToStartNode = Vector2Distance(m_enemy->GetPosition(), node->data);

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
		float distanceToStartNode = Vector2Distance(m_enemy->GetPosition(), node->data);

		if (distanceToStartNode < 16.0f)
		{
			startingNode = node;
			break;
		}
	}

	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {
		float distanceToChosenNode = Vector2Distance(startingNode->data, chosenNode->data);
		
		// If the chosen node is a chaos node, we haven't been to that node, and we are close to it
		if (chosenNode->chaosNode == true && chosenNode->beenHere == false && distanceToChosenNode < 66.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
		});

	if (endNode == nullptr)
	{
		m_player->SetBehaviour(m_wanderBehaviour);
		return chaosPath;
	}

	chaosPath = m_graph->AStarSearch(startingNode, endNode);
	chaosPath.back()->chaosNode = false;

	for (auto node : chaosPath)
	{
		m_seekChaosEmerald->SetPathNode(node->data);
	}

	m_seekChaosEmerald->SetTarget(chaosPath.front()->data);

	return chaosPath;
}

std::list<Graph2D::Node*> Application::PathToMasterNode()
{
	Graph2D::Node* startingNode = nullptr;
	Graph2D::Node* endNode = nullptr;
	std::list<Graph2D::Node*> masterPath;

	for (auto node : m_graph->GetNodes())
	{
		float distanceToStartNode = Vector2Distance(m_enemy->GetPosition(), node->data);

		if (distanceToStartNode < 16.0f)
		{
			startingNode = node;
			break;
		}
	}

	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {

		// If the chosen node is the master node
		if (chosenNode->masterNode == true)
		{
			return true;
		}
		else
		{
			return false;
		}
		});

	masterPath = m_graph->AStarSearch(startingNode, endNode);
	masterPath.back()->beenHere = true;

	for (auto node : masterPath)
	{
		m_seekMasterEmerald->SetPathNode(node->data);
	}

	m_seekMasterEmerald->SetTarget(masterPath.front()->data);

	return masterPath;
}

std::list<Graph2D::Node*> Application::PathToAttackPlayer()
{
	Graph2D::Node* startingNode = nullptr;
	Graph2D::Node* endNode = nullptr;
	std::list<Graph2D::Node*> playerPath;

	for (auto node : m_graph->GetNodes())
	{
		float distanceToStartNode = Vector2Distance(m_enemy->GetPosition(), node->data);

		if (distanceToStartNode < 16.0f)
		{
			startingNode = node;
			break;
		}
	}

	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {

		float distanceToPlayer = Vector2Distance(m_player->GetPosition(), chosenNode->data);

		if (distanceToPlayer < 16.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
		});

	if (endNode == nullptr)
	{
		return playerPath;
	}

	playerPath = m_graph->AStarSearch(startingNode, endNode);

	for (auto node : playerPath)
	{
		m_attackBehaviour->SetPathNode(node->data);
	}

	m_attackBehaviour->SetTarget(playerPath.front()->data);

	return playerPath;
}

std::list<Graph2D::Node*> Application::PathToAvoidPlayer()
{
	Graph2D::Node* startingNode = nullptr;
	Graph2D::Node* endNode = nullptr;
	std::list<Graph2D::Node*> fleePath;

	for (auto node : m_graph->GetNodes())
	{
		float distanceToStartNode = Vector2Distance(m_enemy->GetPosition(), node->data);

		if (distanceToStartNode < 80.0f)
		{
			startingNode = node;
			break;
		}
	}

	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {

		float distanceFromPlayer = Vector2Distance(m_player->GetPosition(), chosenNode->data);

		if (distanceFromPlayer > 80.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
		});

	if (endNode == nullptr)
	{
		return fleePath;
	}

	fleePath = m_graph->AStarSearch(startingNode, endNode);

	for (auto node : fleePath)
	{
		m_fleeBehaviour->SetPathNode(node->data);
	}

	m_fleeBehaviour->SetTarget(fleePath.front()->data);

	return fleePath;
}