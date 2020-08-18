#pragma once

#include "IGameState.h"

#include "raylib.h"

class Application;

class SplashState : public IGameState
{
public:
	SplashState(Application* app);
	virtual ~SplashState();

	virtual void Load();
	virtual void Unload();

	virtual void Update(float dt);
	virtual void Draw();

private:
	Application* m_app;

	Texture2D m_logo;

	float m_timer = 0.0f;
};

