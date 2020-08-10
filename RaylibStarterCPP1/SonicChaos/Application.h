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

	PlayerAgent* m_player = nullptr;
	
	KeyboardBehaviour* m_keyboardBehaviour = nullptr;

	Texture2D m_map;

	Camera2D m_camera;
};

