#pragma once

#include "PlayerAgent.h"
#include "KeyboardBehaviour.h"

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

	void CreateTopWall(float x, float y, float width, float height);
	void CreateLeftWall(float x, float y, float width, float height);
	void CreateRightWall(float x, float y, float width, float height);
	void CreateBottomWall(float x, float y, float width, float height);

	PlayerAgent* m_player = nullptr;
	
	KeyboardBehaviour* m_keyboardBehaviour = nullptr;

	Texture2D m_map;

	Camera2D m_camera;

	std::vector<Rectangle> m_topWalls;
	std::vector<Rectangle> m_leftWalls;
	std::vector<Rectangle> m_rightWalls;
	std::vector<Rectangle> m_bottomWalls;
};

