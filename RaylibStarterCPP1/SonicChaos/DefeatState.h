#pragma once

#include "IGameState.h"

#include "raylib.h"
#include "Button.h"

class Application;

class DefeatState : public IGameState
{
public:
	DefeatState(Application* app);
	virtual ~DefeatState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float dt);
	virtual void Draw();

private:
	Application* m_app;

	Texture2D m_superShadow;

	Button* m_playButton = nullptr;
	Button* m_exitButton = nullptr;
};

