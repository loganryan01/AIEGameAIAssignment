#include "PlayerAgent.h"

PlayerAgent::PlayerAgent()
{
	m_texture = LoadTexture("C:/Users/logan/Desktop/AIEGameAIAssignment/Resources/myTilemap/Sonic.png");
}

PlayerAgent::~PlayerAgent()
{
	UnloadTexture(m_texture);
}

void PlayerAgent::Draw()
{
	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}

int PlayerAgent::GetWidth()
{
	return m_texture.width;
}

int PlayerAgent::GetHeight()
{
	return m_texture.height;
}