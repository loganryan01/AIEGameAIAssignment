#pragma once

#include "IGameState.h"

#include "raylib.h"
#include "Button.h"

class Application;

class MenuState : public IGameState
{
public:
	MenuState(Application* app);
	virtual ~MenuState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float dt);
	virtual void Draw();

private:
	Application* m_app;

	Button* m_playButton = nullptr;
	Button* m_exitButton = nullptr;
};

