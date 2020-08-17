#pragma once

#include "PlayerAgent.h"
#include "EnemyAgent.h"

#include "KeyboardBehaviour.h"
#include "SeekChaosEmerald.h"
#include "SeekMasterEmerald.h"
#include "WanderBehaviour.h"
#include "AttackBehaviour.h"
#include "FleeBehaviour.h"

#include "Graph2D.h"

#include "ChaosEmerald.h"
#include "MasterEmerald.h"

class Application
{
public:
	Application(int windowWidth, int windowHeight);
	~Application();

	void Run();

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Draw();

private:
	Graph2D* LoadGraph();

	std::list<Graph2D::Node*> PathToDoorNode();
	std::list<Graph2D::Node*> PathToChaosNode();
	std::list<Graph2D::Node*> PathToMasterNode();
	std::list<Graph2D::Node*> PathToAttackPlayer();
	std::list<Graph2D::Node*> PathToAvoidPlayer();

private:
	int m_windowWidth;
	int m_windowHeight;
	int m_playerScore = 0;
	int m_enemyScore = 0;

	// Agents
	PlayerAgent* m_player = nullptr;
	EnemyAgent* m_enemy = nullptr;
	
	// Behaviours
	KeyboardBehaviour* m_keyboardBehaviour = nullptr;
	SeekChaosEmerald* m_seekChaosEmerald = nullptr;
	SeekMasterEmerald* m_seekMasterEmerald = nullptr;
	WanderBehaviour* m_wanderBehaviour = nullptr;
	AttackBehaviour* m_attackBehaviour = nullptr;
	FleeBehaviour* m_fleeBehaviour = nullptr;

	// Map
	Texture2D m_map;
	Image m_image;

	// Emeralds
 	std::vector<ChaosEmerald*> m_chaosEmeralds;
	MasterEmerald* m_masterEmerald = nullptr;

	// Graph
	Graph2D* m_graph = nullptr;

	// Camera
	Camera2D m_camera;

	// Pathfinding
	std::list<Graph2D::Node*> m_myPath;
	Graph2D::Node* m_startingNode = nullptr;
	Graph2D::Node* m_endNode = nullptr;
};

