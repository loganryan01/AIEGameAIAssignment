#pragma once

#include "IGameState.h"

#include "raylib.h"
#include "Button.h"

class Application;

class VictoryState : public IGameState
{
public:
	VictoryState(Application* app);
	virtual ~VictoryState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float dt);
	virtual void Draw();

private:
	Application* m_app;

	Texture2D m_superSonic;

	Button* m_exitButton = nullptr;
};

