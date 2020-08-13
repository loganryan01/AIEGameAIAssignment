#pragma once

#include "PlayerAgent.h"
#include "KeyboardBehaviour.h"
#include "SeekChaosEmerald.h"
#include "SeekMasterEmerald.h"
#include "WanderBehaviour.h"
#include "Graph2D.h"

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
	int m_windowWidth;
	int m_windowHeight;

	Graph2D* LoadGraph();

	PlayerAgent* m_player = nullptr;
	
	KeyboardBehaviour* m_keyboardBehaviour = nullptr;
	SeekChaosEmerald* m_seekChaosEmerald = nullptr;
	SeekMasterEmerald* m_seekMasterEmerald = nullptr;
	WanderBehaviour* m_wanderBehaviour = nullptr;

	Texture2D m_map;
	Texture2D m_chaosEmerald1;
	Texture2D m_chaosEmerald2;
	Image m_image;

	Graph2D* m_graph = nullptr;

	Camera2D m_camera;

	std::list<Graph2D::Node*> m_myPath;
	Graph2D::Node* m_startingNode = nullptr;
	Graph2D::Node* m_endNode = nullptr;
};

