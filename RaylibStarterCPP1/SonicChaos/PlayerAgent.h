#pragma once
#include "Agent.h"

class PlayerAgent : public Agent
{
public:
	PlayerAgent();
	virtual ~PlayerAgent();

	virtual void Draw();

	int GetWidth();
	int GetHeight();

private:
	Texture2D m_texture;
};

