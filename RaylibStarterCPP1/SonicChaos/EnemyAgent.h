#pragma once
#include "Agent.h"

class EnemyAgent : public Agent
{
public:
	EnemyAgent();
	virtual ~EnemyAgent();

	virtual void Update(float deltaTime);
	virtual void Draw();

	int GetWidth();
	int GetHeight();

	Rectangle GetTopAABB();
	Rectangle GetLeftAABB();
	Rectangle GetRightAABB();
	Rectangle GetBottomAABB();

private:
	Texture2D m_texture;

	Rectangle m_topAABB;
	Rectangle m_leftAABB;
	Rectangle m_rightAABB;
	Rectangle m_bottomAABB;
};

