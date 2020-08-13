#include "ChaosEmerald.h"

ChaosEmerald::ChaosEmerald()
{
	m_texture = { 0, 0, 0, 0, 0 };
	m_position = { 0,0 };
}

void ChaosEmerald::Draw()
{
	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}