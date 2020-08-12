#include "PlayerAgent.h"

PlayerAgent::PlayerAgent()
{
	m_texture = LoadTexture("./resources/Shadow.png");

	m_topAABB = { m_position.x, m_position.y, (float)m_texture.width, 1.0f };
	m_leftAABB = { m_position.x, m_position.y, 1.0f, (float)m_texture.height };
	m_rightAABB = { m_position.x + (float)m_texture.width, m_position.y, 1.0f, (float)m_texture.height };
	m_bottomAABB = { m_position.x, m_position.y + ((float)m_texture.height - 1.0f), (float)m_texture.width, 1.0f};
}

PlayerAgent::~PlayerAgent()
{
	UnloadTexture(m_texture);
}

void PlayerAgent::Update(float deltaTime)
{
	m_topAABB = { m_position.x, m_position.y, (float)m_texture.width, 1.0f };
	m_leftAABB = { m_position.x, m_position.y, 1.0f, (float)m_texture.height };
	m_rightAABB = { m_position.x + (float)m_texture.width, m_position.y, 1.0f, (float)m_texture.height };
	m_bottomAABB = { m_position.x, m_position.y + ((float)m_texture.height - 1.0f), (float)m_texture.width, 1.0f };
	
	Agent::Update(deltaTime);
}

void PlayerAgent::Draw()
{
	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
	DrawRectangleLines(m_topAABB.x, m_topAABB.y, m_topAABB.width, m_topAABB.height, BLACK);
	DrawRectangleLines(m_leftAABB.x, m_leftAABB.y, m_leftAABB.width, m_leftAABB.height, BLACK);
	DrawRectangleLines(m_rightAABB.x, m_rightAABB.y, m_rightAABB.width, m_rightAABB.height, BLACK);
	DrawRectangleLines(m_bottomAABB.x, m_bottomAABB.y, m_bottomAABB.width, m_bottomAABB.height, BLACK);
}

int PlayerAgent::GetWidth()
{
	return m_texture.width;
}

int PlayerAgent::GetHeight()
{
	return m_texture.height;
}

Rectangle PlayerAgent::GetTopAABB()
{
	return m_topAABB;
}

Rectangle PlayerAgent::GetLeftAABB()
{
	return m_leftAABB;
}

Rectangle PlayerAgent::GetRightAABB()
{
	return m_rightAABB;
}

Rectangle PlayerAgent::GetBottomAABB()
{
	return m_bottomAABB;
}