/*--------------------------------------------
	File Name: PlayState.cpp
	Purpose: Launch the main part of the game.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------*/

#include "PlayState.h"

#include "Application.h"
#include "GameStateManager.h"

//-----------------------------------------------------
// Constructor
//	app (Application*): Which application are we using?
//-----------------------------------------------------
PlayState::PlayState(Application* app) : m_app(app)
{
	m_map = { 0 };
	m_image = { 0 };
	m_camera = { 0 };
}

//-----------
// Destructor
//-----------
PlayState::~PlayState()
{

}

//--------------------------------------
// Load the assets needed for this state
//--------------------------------------
void PlayState::Load()
{
	// Load the map
	m_image = LoadImage("./resources/tilemap.png");
	m_map = LoadTextureFromImage(m_image);

	// Load the Chaos Emeralds
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

	// Load the Master Emerald
	m_masterEmerald = new MasterEmerald();
	m_masterEmerald->SetTexture(LoadTexture("./resources/Master Emerald.png"));
	m_masterEmerald->SetPosition({ 384 , 16 });

	// Load the Graph
	m_graph = LoadGraph();

	// Load the Agents
	m_player = new PlayerAgent();
	m_player->SetPosition({ 424, 760 });

	m_enemy = new EnemyAgent();
	m_enemy->SetPosition({ 392, 760 });

	// Load the Behaviours
	m_keyboardBehaviour = new KeyboardBehaviour();
	m_seekChaosEmerald = new SeekChaosEmerald();
	m_seekMasterEmerald = new SeekMasterEmerald();
	m_wanderBehaviour = new WanderBehaviour();
	m_attackBehaviour = new AttackBehaviour();
	m_fleeBehaviour = new FleeBehaviour();

	// Set the behaviours for the agents
	m_player->SetBehaviour(m_keyboardBehaviour);
	m_enemy->SetBehaviour(m_wanderBehaviour);

	// Set up the Camera
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
	m_camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
	m_camera.zoom = 5.0f;

	// Calculate starting path
	m_myPath = PathToDoorNode();
}

//-----------------------------------------------
// Unload the assets that was used for this state
//-----------------------------------------------
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

//--------------------------------------------------------------
// Update the state
//	deltaTime (float): How many frames are happening per second?
//--------------------------------------------------------------
void PlayState::Update(float deltaTime)
{
	// Switch Behaviours if needed
	BehaviourSwitching();

	//----- Pathfinding -----
	// If we are at the end of the path or we don't have a path at all
	if (m_myPath.empty() || m_distanceToEndOfPath < 12.0f)
	{
		// Empty the path
		m_myPath.clear();

		// If the enemy is wandering
		if (m_enemy->GetBehaviour() == m_wanderBehaviour)
		{
			// Calculate a path to a door node
			m_myPath = PathToDoorNode();
		}
		// If the enemy is seeking a chaos emerald
		else if (m_enemy->GetBehaviour() == m_seekChaosEmerald)
		{
			// Calculate a path to the chaos emerald node
			m_myPath = PathToChaosNode();
		}
		// If the enemy is seeking the master emerald
		else if (m_enemy->GetBehaviour() == m_seekMasterEmerald)
		{
			// Calculate a path to the master emerald node
			m_myPath = PathToMasterNode();
		}
		// If the enemy is attacking the player
		else if (m_enemy->GetBehaviour() == m_attackBehaviour)
		{
			// Calculate a path to the player
			m_myPath = PathToAttackPlayer();

			// If the player is not on the path
			if (m_myPath.empty())
			{
				// Center of the player texture
				float playerXPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
				float playerYPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

				// Center of the enemy texture
				float enemyXPos = m_enemy->GetPosition().x + m_enemy->GetWidth() / 2.0f;
				float enemyYPos = m_enemy->GetPosition().y + m_enemy->GetHeight() / 2.0f;

				// Distance from the enemy
				float distanceFromPlayer = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

				// If the enemy can see the player, got to him/her
				if (distanceFromPlayer < 80.0f)
				{
					m_attackBehaviour->SetTarget({ playerXPos, playerYPos });
				}
			}
		}
		// If the enemy is fleeing from the player
		else if (m_enemy->GetBehaviour() == m_fleeBehaviour)
		{
			// Calculate a path to avoid the player
			m_myPath = PathToAvoidPlayer();
		}
	}

	//----- Collision -----
	// Loop through all the barricades and check if the player or the enemy is touching either one of them
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
	// Charge the attack bar of the player and the enemy if it is not fully charged
	if (m_player->GetAttackCharger() < m_maxAttackCharge)
	{
		m_player->IncrementAttackCharger();
	}

	if (m_enemy->GetAttackCharger() < m_maxAttackCharge)
	{
		m_enemy->IncrementAttackCharger();
	}

	// If the player has decided to attack and the attack charger is full
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

		// If the enemy is within range of the player
		float distanceFromEnemy = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

		if (distanceFromEnemy < m_player->GetAttackRadius())
		{
			m_enemyHit = true;

			// Take away a chaos emerald if they have one
			if (m_enemyScore > 0)
			{
				m_playerScore++;
				m_enemyScore--;
			}
		}
	}

	//----- Stun Behaviour -----
	// If the player has been hit
	if (m_playerHit)
	{
		// Increase the player's stun time by 1
		m_playerStunTime++;
		// Set the velocity to 0
		m_player->SetVelocity({ 0 ,0 });

		// Once the time is up
		if (m_playerStunTime == m_maxStunTime)
		{
			// Reset
			m_playerStunTime = 0;
			m_playerHit = false;
		}
	}

	// Similar to the code above
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

	//----- Increasing player score -----
	// Loop through all the chaos emeralds
	for (int i = 0; i < m_chaosEmeralds.size(); i++)
	{
		// If the player is close to a chaos emerald
		float distanceToChaosEmerald = Vector2Distance(m_player->GetPosition(), m_chaosEmeralds[i]->GetPosition());

		if (distanceToChaosEmerald < 44.0f)
		{
			// Get the node that is close to the chaos emerald
			for (auto node : m_graph->GetNodes())
			{
				if (node->chaosNode == true)
				{
					float distanceToChaosNode = Vector2Distance(m_player->GetPosition(), node->data);

					if (distanceToChaosNode < 66.0f)
					{
						// Remove it
						node->chaosNode = false;
					}
				}
			}

			// Remove the chaos emerald and increase the player's score by 1
			m_chaosEmeralds.erase(m_chaosEmeralds.begin() + i);
			m_playerScore++;
			break;
		}
	}

	m_player->Update(deltaTime);
	m_enemy->Update(deltaTime);

	//----- State Changing -----
	float enemyDistanceToMasterEmerald = Vector2Distance(m_enemy->GetPosition(), { m_masterEmerald->GetPosition().x + m_masterEmerald->GetTexture().width / 2.0f, m_masterEmerald->GetPosition().y + m_masterEmerald->GetTexture().height / 2.0f });
	float playerDistanceToMasterEmerald = Vector2Distance(m_player->GetPosition(), { m_masterEmerald->GetPosition().x + m_masterEmerald->GetTexture().width / 2.0f, m_masterEmerald->GetPosition().y + m_masterEmerald->GetTexture().height / 2.0f });

	// Switch to the victory state if the player is close the Master Emerald and they have all the Chaos Emeralds
	if (playerDistanceToMasterEmerald < 48.0f && m_playerScore == 7)
	{
		m_app->GetGameStateManager()->SetState("Play", nullptr);
		m_app->GetGameStateManager()->PopState();
		m_app->GetGameStateManager()->PushState("Victory");
	}

	// Switch to the defeat state if the enemy is close the Master Emerald and they have all the Chaos Emeralds
	if (enemyDistanceToMasterEmerald < 48.0f && m_enemyScore == 7)
	{
		m_app->GetGameStateManager()->SetState("Play", nullptr);
		m_app->GetGameStateManager()->PopState();
		m_app->GetGameStateManager()->PushState("Defeat");
	}

	//----- Door node -----
	float beenHereDoorNodes = 0.0f;

	// Loop through all the nodes
	for (auto node : m_graph->GetNodes())
	{
		if (node->doorNode == true && node->beenHere == true)
		{
			// Increase by 1 if it's a door node that the enemy has been to it
			beenHereDoorNodes++;
		}
	}

	// If the enemy has been to all the door nodes, reset them
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

	//----- Debug mode -----
	// Press tab to activate debug mode
	if (IsKeyPressed(KEY_TAB) && m_debugMode == false)
	{
		m_debugMode = true;
	}
	else if (IsKeyPressed(KEY_TAB) && m_debugMode == true)
	{
		m_debugMode = false;
	}

	//----- Camera -----
	// Update the camera position
	m_camera.target = { m_player->GetPosition().x + m_player->GetWidth() / 2, m_player->GetPosition().y + m_player->GetHeight() / 2 };
}

//---------------
// Draw the state
//---------------
void PlayState::Draw()
{
	// Use the camera if we are not in debug mode
	if (m_debugMode == false)
	{
		BeginMode2D(m_camera);
	}

	DrawTexture(m_map, 0, 0, WHITE);

	// Draw a circle around the agents when they attack
	if (m_enemy->GetAttackCharger() < 60.0f)
	{
		for (int i = 0; i < 60; i++)
		{
			DrawCircle((int)m_enemy->GetPosition().x + m_enemy->GetWidth() / 2, (int)m_enemy->GetPosition().y + m_enemy->GetHeight() / 2, m_enemy->GetAttackRadius(), PURPLE);
		}
	}

	if (m_player->GetAttackCharger() < 60.0f)
	{
		for (int i = 0; i < 60; i++)
		{
			DrawCircle((int)m_player->GetPosition().x + m_player->GetWidth() / 2, (int)m_player->GetPosition().y + m_player->GetHeight() / 2, m_player->GetAttackRadius(), BLUE);
		}
	}

	// Draw Game Objects
	for (int i = 0; i < m_chaosEmeralds.size(); i++)
	{
		m_chaosEmeralds[i]->Draw();
	}

	m_masterEmerald->Draw();

	// Draw Agents
	m_player->Draw();
	m_enemy->Draw();

	// Draw the graph when we are in debug mode and the AI path
	if (m_debugMode == true)
	{
		m_graph->Draw();
		m_graph->DrawPath(m_myPath);
	}

	// If we are not in debug mode, then leave 2D mode
	if (m_debugMode == false)
	{
		EndMode2D();
	}

	// Tell the player their attack is ready, once the attack is charged
	if (m_player->GetAttackCharger() == m_maxAttackCharge)
	{
		DrawText("Attack is Ready", 10, 10, 14, BLACK);
	}

	char playerScore[5];
	char enemyScore[5];
	_itoa(m_playerScore, playerScore, 10);
	_itoa(m_enemyScore, enemyScore, 10);

	// Draw scores
	DrawText("Player's score: ", 10, 30, 14, BLACK);
	DrawText("Enemy's score: ", 10, 50, 14, BLACK);
	DrawText(playerScore, 120, 30, 14, BLACK);
	DrawText(enemyScore, 120, 50, 14, BLACK);
}

//---------------
// Load the graph
//---------------
Graph2D* PlayState::LoadGraph()
{
	// Get the graph
	Image image = LoadImage("./resources/graph.png");

	// Get the pixels from the image
	Color* pixels = GetImageData(image);

	Graph2D* graph = new Graph2D;
	std::vector<Graph2D::Node*> nodes;

	int tileWidth = 16;
	int tileHeight = 16;

	// Loop through the pixels
	for (int y = 0; y < m_image.height; y += tileHeight)
	{
		for (int x = 0; x < m_image.width; x += tileWidth)
		{
			float xPos = (float)x + 8;
			float yPos = (float)y + 8;

			// Get pixel color from texture
			auto c = pixels[y * m_image.width + x];

			// Create the nodes based on their colour
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

	// Create edges
	for (auto node : graph->GetNodes())
	{
		graph->SetEdges(node, 32.0f);
	}

	// Return graph
	return graph;
}

//--------------------------
// Get a path to a door node
//--------------------------
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

	// If a starting node cannot be found then return an empty path
	try {
		if (startingNode == nullptr)
		{
			throw startingNode;
		}
	}
	catch (...) {
		return doorPath;
	}

	// Get the a door node that the enemy has not been to
	endNode = m_graph->DepthFirstSearch(startingNode, [&](Graph2D::Node* chosenNode) {
		float distance = Vector2Distance(startingNode->data, chosenNode->data);

		if (chosenNode->doorNode == true && chosenNode->beenHere == false && distance >= 32.0f)
		{
			return true;
		}

		return false;
		});

	// If an end node cannot be found then return an empty path and set behaviour to wander
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

	// Calculate a path
	doorPath = m_graph->dijkstrasSearch(startingNode, endNode);

	// Set the end node's been here to true
	doorPath.back()->beenHere = true;

	// Set the path for the behaviour
	for (auto node : doorPath)
	{
		m_wanderBehaviour->SetPathNode(node->data);
	}

	// Set the target to be the front of the path
	m_wanderBehaviour->SetTarget(doorPath.front()->data);

	return doorPath;
}

//-----------------------------------
// Get a path to a Chaos Emerald node
//-----------------------------------
std::list<Graph2D::Node*> PlayState::PathToChaosNode()
{
	// Similar code to PathToDoorNode but the difference in this one is it looks for a Chaos Emerald node
	
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

	// Remove the chaos node from the graph
	chaosPath.back()->chaosNode = false;

	for (auto node : chaosPath)
	{
		m_seekChaosEmerald->SetPathNode(node->data);
	}

	m_seekChaosEmerald->SetTarget(chaosPath.front()->data);

	return chaosPath;
}

//--------------------------------------
// Get a path to the Master Emerald node
//--------------------------------------
std::list<Graph2D::Node*> PlayState::PathToMasterNode()
{
	// Similar code to PathToDoorNode but the difference in this one is it looks for the Master Emerald node

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

//-----------------------------------------------------------
// Get a path to the node that player is on to attack him/her
//-----------------------------------------------------------
std::list<Graph2D::Node*> PlayState::PathToAttackPlayer()
{
	// Similar code to PathToDoorNode but the difference in this one is it looks for the node that the player is on
	
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

//--------------------------------------------------
// Get a path to a node that is away from the player
//--------------------------------------------------
std::list<Graph2D::Node*> PlayState::PathToAvoidPlayer()
{
	// Similar code to PathToDoorNode but the difference in this one is it looks for the node that is far away from the player
	
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

//--------------------------------------
// Switch Behaviours for the enemy agent
//--------------------------------------
void PlayState::BehaviourSwitching()
{
	// Check if we are at the end of the path
	if (!m_myPath.empty())
	{
		m_distanceToEndOfPath = Vector2Distance(m_enemy->GetPosition(), m_myPath.back()->data);
	}

	// If we are at the end of the path.
	if (m_distanceToEndOfPath < 12.0f)
	{
		// If the enemy's behaviour is wander
		if (m_enemy->GetBehaviour() == m_wanderBehaviour)
		{
			// Loop through all the nodes
			for (auto node : m_graph->GetNodes())
			{
				float distanceToChaosNode = Vector2Distance(m_enemy->GetPosition(), node->data);

				// If the enemy is close to a Chaos Emerald, switch to seek Chaos Emerald
				if (node->chaosNode == true && node->beenHere == false && distanceToChaosNode < 78.0f)
				{
					m_enemy->SetBehaviour(m_seekChaosEmerald);
					break;
				}
			}

			float distanceToPlayer = Vector2Distance(m_enemy->GetPosition(), m_player->GetPosition());

			// If the enemy has all the Chaos Emeralds, switch to seek Master Emerald
			if (m_enemyScore == 7)
			{
				m_enemy->SetBehaviour(m_seekMasterEmerald);
			}
			// If the enemy doesn't have all the chaos emeralds, can see the player and their attack is ready
			else if (m_enemyScore + m_playerScore == 7 && distanceToPlayer < 80.0f && m_enemy->GetAttackCharger() == m_maxAttackCharge)
			{
				// Switch to attack 
				m_enemy->SetBehaviour(m_attackBehaviour);
			}
		}
		// If the enemy's behaviour is seek chaos emerald
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

			// If the enemy has all the chaos emeralds, switch to seek Master Emerald
			if (m_enemyScore == 7)
			{
				m_enemy->SetBehaviour(m_seekMasterEmerald);
			}
		}
		// If the enemy's behaviour is seek Master Emerald
		else if (m_enemy->GetBehaviour() == m_seekMasterEmerald)
		{
			// And doesn't have the 7 Chaos Emeralds, switch to seek Master Emerald
			if (m_enemyScore < 7)
			{
				m_enemy->SetBehaviour(m_wanderBehaviour);
			}
		}
	}

	// If the enemy's behaviour is attack
	if (m_enemy->GetBehaviour() == m_attackBehaviour)
	{
		// Center of the player texture
		float playerXPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
		float playerYPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

		// Center of the enemy texture
		float enemyXPos = m_enemy->GetPosition().x + m_enemy->GetWidth() / 2.0f;
		float enemyYPos = m_enemy->GetPosition().y + m_enemy->GetHeight() / 2.0f;

		float distanceToPlayer = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

		// If the enemy has hit the player
		if (distanceToPlayer < m_enemy->GetAttackRadius())
		{
			// Reset the attack charger
			m_enemy->ResetAttackCharger();
			m_playerHit = true;

			// Steal an emerald if the player has one
			if (m_playerScore > 0)
			{
				m_playerScore--;
				m_enemyScore++;
			}
		}

		// Switch to flee once we have attacked, or wander if we are already far away from the player
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
	// If the enemy's behaviour is flee
	else if (m_enemy->GetBehaviour() == m_fleeBehaviour)
	{
		// Center of the player texture
		float playerXPos = m_player->GetPosition().x + m_player->GetWidth() / 2.0f;
		float playerYPos = m_player->GetPosition().y + m_player->GetHeight() / 2.0f;

		// Center of the enemy texture
		float enemyXPos = m_enemy->GetPosition().x + m_enemy->GetWidth() / 2.0f;
		float enemyYPos = m_enemy->GetPosition().y + m_enemy->GetHeight() / 2.0f;

		float distanceFromPlayer = Vector2Distance({ playerXPos, playerYPos }, { enemyXPos, enemyYPos });

		// If the enemy has escaped the player then switch to wander behaviour
		if (distanceFromPlayer > 80.0f)
		{
			m_enemy->SetBehaviour(m_wanderBehaviour);
			m_myPath.clear();
		}
	}
}