#pragma once

#include "PlayerAgent.h"
#include "KeyboardBehaviour.h"
#include "SeekChaosEmerald.h"
#include "SeekMasterEmerald.h"
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

	void LoadLeftWall();
	void LoadRightWall();

	Graph2D* LoadGraph();

	void CreateTopWall(float x, float y, float width, float height);
	void CreateLeftWall(float x, float y, float width, float height);
	void CreateRightWall(float x, float y, float width, float height);
	void CreateBottomWall(float x, float y, float width, float height);

	PlayerAgent* m_player = nullptr;
	
	KeyboardBehaviour* m_keyboardBehaviour = nullptr;
	SeekChaosEmerald* m_seekChaosEmerald = nullptr;
	SeekMasterEmerald* m_seekMasterEmerald = nullptr;

	Texture2D m_map;
	Image m_image;

	Graph2D* m_graph = nullptr;

	Camera2D m_camera;

	std::vector<Rectangle> m_topWalls;
	std::vector<Rectangle> m_leftWalls;
	std::vector<Rectangle> m_rightWalls;
	std::vector<Rectangle> m_bottomWalls;

	std::list<Graph2D::Node*> m_myPath;
};

