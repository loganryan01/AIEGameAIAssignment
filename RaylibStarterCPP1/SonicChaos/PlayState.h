#pragma once

#include "IGameState.h"

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

class Application;

class PlayState : public IGameState
{
public:
	PlayState(Application* app);
	virtual ~PlayState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float dt);
	virtual void Draw();
private:
	Graph2D* LoadGraph();

	std::list<Graph2D::Node*> PathToDoorNode();
	std::list<Graph2D::Node*> PathToChaosNode();
	std::list<Graph2D::Node*> PathToMasterNode();
	std::list<Graph2D::Node*> PathToAttackPlayer();
	std::list<Graph2D::Node*> PathToAvoidPlayer();

	void BehaviourSwitching();

private:
	Application* m_app;

	int m_playerScore = 0;
	int m_enemyScore = 0;

	float m_playerStunTime = 0.0f;
	float m_enemyStunTime = 0.0f;

	bool m_playerHit = false;
	bool m_enemyHit = false;

	float m_distanceToEndOfPath = 0.0f;
	float m_maxAttackCharge = 600.0f;
	float m_maxStunTime = 300.0f;
	float m_numberOfDoorNodes = 55.0f;

	bool m_debugMode = false;

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

