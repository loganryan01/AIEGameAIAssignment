/*--------------------------------------------------------------------
	File Name: PlayState.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

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
	// Constructor
	PlayState(Application* app);

	// Destructor
	virtual ~PlayState();

	// Load the assets needed for this state
	virtual void Load();

	// Unload the assets that was used for this state
	virtual void Unload();

	// Update the state
	virtual void Update(float dt);

	// Draw the state
	virtual void Draw();
private:
	// Load the graph
	Graph2D* LoadGraph();

	// Get a path to a door node
	std::list<Graph2D::Node*> PathToDoorNode();

	// Get a path to a Chaos Emerald node
	std::list<Graph2D::Node*> PathToChaosNode();

	// Get a path to the Master Emerald node
	std::list<Graph2D::Node*> PathToMasterNode();

	// Get a path to the node that player is on to attack him/her
	std::list<Graph2D::Node*> PathToAttackPlayer();

	// Get a path to a node that is away from the player
	std::list<Graph2D::Node*> PathToAvoidPlayer();

	// Switch Behaviours for the enemy agent
	void BehaviourSwitching();

private:
	// Pointer to the application
	Application* m_app;

	// Player's score
	int m_playerScore = 0;

	// Enemy's score
	int m_enemyScore = 0;

	// Player stun time
	float m_playerStunTime = 0.0f;

	// Enemy stun time
	float m_enemyStunTime = 0.0f;

	// Has the player been hit?
	bool m_playerHit = false;

	// Has the enemy been hit?
	bool m_enemyHit = false;

	// Has the player chosen to attack
	bool m_playerAttack = false;

	// Distance to the end of the path
	float m_distanceToEndOfPath = 0.0f;

	// Maximum charge needed for an attack
	float m_maxAttackCharge = 6000.0f;

	// Maximum stun time for an agent
	float m_maxStunTime = 3000.0f;

	// Number of door nodes on the map
	float m_numberOfDoorNodes = 55.0f;

	// Is the game in debug mode
	bool m_debugMode = false;

	// Player agent
	PlayerAgent* m_player = nullptr;

	// Enemy agent
	EnemyAgent* m_enemy = nullptr;

	// Keyboard behaviour
	KeyboardBehaviour* m_keyboardBehaviour = nullptr;

	// Seek Chaos Emerald behaviour
	SeekChaosEmerald* m_seekChaosEmerald = nullptr;

	// Seek Master Emerald behaviour
	SeekMasterEmerald* m_seekMasterEmerald = nullptr;

	// Wander behaviour
	WanderBehaviour* m_wanderBehaviour = nullptr;

	// Attack behaviour
	AttackBehaviour* m_attackBehaviour = nullptr;

	// Flee behaviour
	FleeBehaviour* m_fleeBehaviour = nullptr;

	// Texture of the map
	Texture2D m_map;

	// Image of the map
	Image m_image;

	// Vector for the Chaos Emeralds
	std::vector<ChaosEmerald*> m_chaosEmeralds;

	// Master Emerald
	MasterEmerald* m_masterEmerald = nullptr;

	// Graph
	Graph2D* m_graph = nullptr;

	// Camera
	Camera2D m_camera;

	// Enemy agent path
	std::list<Graph2D::Node*> m_myPath;

	// Starting node for the path
	Graph2D::Node* m_startingNode = nullptr;

	// Ending node for the path
	Graph2D::Node* m_endNode = nullptr;
};

