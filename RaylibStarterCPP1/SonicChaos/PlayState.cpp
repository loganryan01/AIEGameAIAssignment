#include "PlayState.h"

PlayState::PlayState(Application* app) : m_app(app)
{
	m_map = { 0 };
	m_image = { 0 };
	m_camera = { 0 };
}

PlayState::~PlayState()
{

}

void PlayState::Load()
{
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
	m_camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
	m_camera.zoom = 5.0f;

	// Calculate starting path
	m_myPath = PathToDoorNode();
}

void PlayState::Unload()
{
	// Unload behaviours
	delete m_keyboardBehaviour;
	delete m_seekChaosEmerald;
	delete m_seekMasterEmerald;
	delete m_wanderBehaviour;
	delete m_attackBehaviour;
	delete m_fleeBehaviour;

	// Unload game objects
	for (auto chaosEmerald : m_chaosEmeralds)
	{
		delete chaosEmerald;
	}
	delete m_masterEmerald;

	// Unload Agents
	delete m_player;
	delete m_enemy;

	// Unload graph
	delete m_graph;

	// Unload map
	UnloadTexture(m_map);
}

void PlayState::Update(float deltaTime)
{
	// Switch Behaviours if needed
	BehaviourSwitching();

	//----- Pathfinding -----
	// Check if we have a path or at the end of the path
	if (m_myPath.empty() || m_distanceToEndOfPath < 12.0f)
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

	//----- Collision -----
	for (auto barricade : m_graph->GetBarricades())
	{
		if (CheckCollisionCircleRec(barricade, 8.0f, m_player->GetTopAABB()) && m_player->GetVelocity().y < 0)
		{
			m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
		}

		if (CheckCollisionCircleRec(barricade, 8.0f, m_player->GetBottomAABB()) && m_player->GetVelocity().y > 0)
		{
			m_player->SetVelocity({ m_player->GetVelocity().x, 0 });
		}

		if (CheckCollisionCircleRec(barricade, 8.0f, m_player->GetLeftAABB()) && m_player->GetVelocity().x < 0)
		{
			m_player->SetVelocity({ 0, m_player->GetVelocity().y });
		}

		if (CheckCollisionCircleRec(barricade, 8.0f, m_player->GetRightAABB()) && m_player->GetVelocity().x > 0)
		{
			m_player->SetVelocity({ 0, m_player->GetVelocity().y });
		}

		if (CheckCollisionCircleRec(barricade, 8.0f, m_enemy->GetTopAABB()) && m_enemy->GetVelocity().y < 0)
		{
			m_enemy->SetVelocity({ m_enemy->GetVelocity().x, 0 });
		}

		if (CheckCollisionCircleRec(barricade, 8.0f, m_enemy->GetBottomAABB()) && m_enemy->GetVelocity().y > 0)
		{
			m_enemy->SetVelocity({ m_enemy->GetVelocity().x, 0 });
		}

		if (CheckCollisionCircleRec(barricade, 8.0f, m_enemy->GetLeftAABB()) && m_enemy->GetVelocity().x < 0)
		{
			m_enemy->SetVelocity({ 0, m_enemy->GetVelocity().y });
		}

		if (CheckCollisionCircleRec(barricade, 8.0f, m_enemy->GetRightAABB()) && m_enemy->GetVelocity().x > 0)
		{
			m_enemy->SetVelocity({ 0, m_enemy->GetVelocity().y });
		}
	}

	//----- Attack -----
	// Charge the attack bar if it is not fully charged
	if (m_player->GetAttackCharger() < m_maxAttackCharge)
	{
		m_player->IncrementAttackCharger();
	}

	if (m_enemy->GetAttackCharger() < m_maxAttackCharge)
	{
		m_enemy->IncrementAttackCharger();
	}

	// Remove a chaos emerald if there is one and the player is close to it.
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

	if (IsKeyPressed(KEY_SPACE) && m_player->GetAttackCharger() == m_maxAttackCharge)
	{
		m_player->ResetAttackCharger();
		m_playerAttack = true;

		// Center of the player texture
		float playerXPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
		float playerYPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

		// Center of the enemy texture
		float enemyXPos = m_enemy->GetPosition().x + m_enemy->GetWidth() / 2.0f;
		float enemyYPos = m_enemy->GetPosition().y + m_enemy->GetHeight() / 2.0f;

		float distanceFromEnemy = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

		if (distanceFromEnemy < m_player->GetAttackRadius())
		{
			m_enemyHit = true;

			if (m_enemyScore > 0)
			{
				m_playerScore++;
				m_enemyScore--;
			}
		}
	}

	if (m_playerHit)
	{
		m_playerStunTime++;
		m_player->SetVelocity({ 0 ,0 });

		if (m_playerStunTime == m_maxStunTime)
		{
			m_playerStunTime = 0;
			m_playerHit = false;
		}
	}

	if (m_enemyHit)
	{
		m_enemyStunTime++;
		m_enemy->SetVelocity({ 0 ,0 });

		if (m_enemyStunTime == m_maxStunTime)
		{
			m_enemyStunTime = 0;
			m_enemyHit = false;
		}
	}

	// Reset the door nodes 
	float beenHereDoorNodes = 0.0f;

	for (auto node : m_graph->GetNodes())
	{
		if (node->doorNode == true && node->beenHere == true)
		{
			beenHereDoorNodes++;
		}
	}

	if (beenHereDoorNodes == m_numberOfDoorNodes)
	{
		for (auto node : m_graph->GetNodes())
		{
			if (node->doorNode == true && node->beenHere == true)
			{
				node->beenHere = false;
			}
		}
	}

	if (IsKeyPressed(KEY_TAB) && m_debugMode == false)
	{
		m_debugMode = true;
	}
	else if (IsKeyPressed(KEY_TAB) && m_debugMode == true)
	{
		m_debugMode = false;
	}

	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
}

void PlayState::Draw()
{
	if (m_debugMode == false)
	{
		BeginMode2D(m_camera);
	}

	DrawTexture(m_map, 0, 0, WHITE);

	if (m_enemy->GetAttackCharger() < 60.0f)
	{
		for (int i = 0; i < 60; i++)
		{
			DrawCircle(m_enemy->GetPosition().x + m_enemy->GetWidth() / 2, m_enemy->GetPosition().y + m_enemy->GetHeight() / 2, m_enemy->GetAttackRadius(), PURPLE);
		}
	}

	if (m_player->GetAttackCharger() < 60.0f)
	{
		for (int i = 0; i < 60; i++)
		{
			DrawCircle(m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2, m_player->GetAttackRadius(), BLUE);
		}
	}

	for (int i = 0; i < m_chaosEmeralds.size(); i++)
	{
		m_chaosEmeralds[i]->Draw();
	}

	m_masterEmerald->Draw();

	m_player->Draw();
	m_enemy->Draw();

	if (m_debugMode == true)
	{
		m_graph->Draw();
		m_graph->DrawPath(m_myPath);
	}

	if (m_debugMode == false)
	{
		EndMode2D();
	}

	if (m_player->GetAttackCharger() == m_maxAttackCharge)
	{
		DrawText("Attack is Ready", 10, 10, 14, BLACK);
	}

	char playerScore[5];
	char enemyScore[5];
	char playerAttack[10];
	_itoa(m_player->GetAttackCharger(), playerAttack, 10);
	_itoa(m_playerScore, playerScore, 10);
	_itoa(m_enemyScore, enemyScore, 10);

	DrawText(playerAttack, 10, 70, 14, BLACK);
	DrawText("Player's score: ", 10, 30, 14, BLACK);
	DrawText("Enemy's score: ", 10, 50, 14, BLACK);
	DrawText(playerScore, 120, 30, 14, BLACK);
	DrawText(enemyScore, 120, 50, 14, BLACK);
}

Graph2D* PlayState::LoadGraph()
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

std::list<Graph2D::Node*> PlayState::PathToDoorNode()
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

	try {
		if (startingNode == nullptr)
		{
			throw startingNode;
		}
	}
	catch (...) {
		return doorPath;
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

	try {
		if (endNode == nullptr)
		{
			throw endNode;
		}
	}
	catch (...) {
		m_player->SetBehaviour(m_wanderBehaviour);
		return doorPath;
	}

	doorPath = m_graph->dijkstrasSearch(startingNode, endNode);
	doorPath.back()->beenHere = true;

	for (auto node : doorPath)
	{
		m_wanderBehaviour->SetPathNode(node->data);
	}

	m_wanderBehaviour->SetTarget(doorPath.front()->data);

	return doorPath;
}

std::list<Graph2D::Node*> PlayState::PathToChaosNode()
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

	try {
		if (startingNode == nullptr)
		{
			throw startingNode;
		}
	}
	catch (...) {
		return chaosPath;
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

	try {
		if (endNode == nullptr)
		{
			throw endNode;
		}
	}
	catch (...) {
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

std::list<Graph2D::Node*> PlayState::PathToMasterNode()
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

	try {
		if (startingNode == nullptr)
		{
			throw startingNode;
		}
	}
	catch (...) {
		return masterPath;
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

	try {
		if (endNode == nullptr)
		{
			throw endNode;
		}
	}
	catch (...) {
		return masterPath;
	}

	masterPath = m_graph->AStarSearch(startingNode, endNode);
	masterPath.back()->beenHere = true;

	for (auto node : masterPath)
	{
		m_seekMasterEmerald->SetPathNode(node->data);
	}

	m_seekMasterEmerald->SetTarget(masterPath.front()->data);

	return masterPath;
}

std::list<Graph2D::Node*> PlayState::PathToAttackPlayer()
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

	try {
		if (startingNode == nullptr)
		{
			throw startingNode;
		}
	}
	catch (...) {
		return playerPath;
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

	try {
		if (endNode == nullptr)
		{
			throw endNode;
		}
	}
	catch (...) {
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

std::list<Graph2D::Node*> PlayState::PathToAvoidPlayer()
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

	try {
		if (startingNode == nullptr)
		{
			throw startingNode;
		}
	}
	catch (...) {
		return fleePath;
	}

	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {

		float distanceFromPlayer = Vector2Distance(m_player->GetPosition(), chosenNode->data);

		if (distanceFromPlayer > 160.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
		});

	try {
		if (endNode == nullptr)
		{
			throw endNode;
		}
	}
	catch (...) {
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

void PlayState::BehaviourSwitching()
{
	// Check if we are at the end of the path
	if (!m_myPath.empty())
	{
		m_distanceToEndOfPath = Vector2Distance(m_enemy->GetPosition(), m_myPath.back()->data);
	}

	//----- Behaviour switching -----
	if (m_distanceToEndOfPath < 12.0f)
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
			else if (m_enemyScore + m_playerScore == 7 && distanceToPlayer < 80.0f && m_enemy->GetAttackCharger() == m_maxAttackCharge)
			{
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
			m_playerHit = true;

			if (m_playerScore > 0)
			{
				m_playerScore--;
				m_enemyScore++;
			}
		}

		// Switch to flee once we have attacked
		if (m_enemy->GetAttackCharger() < m_maxAttackCharge)
		{
			m_enemy->SetBehaviour(m_fleeBehaviour);
			m_myPath.clear();
		}
		else if (distanceToPlayer > 80.0f)
		{
			m_enemy->SetBehaviour(m_wanderBehaviour);
			m_myPath.clear();
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
			m_myPath.clear();
		}
	}
}