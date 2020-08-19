/*--------------------------------
	File Name: ChaosEmerald.cpp
	Purpose: Draw a Chaos Emerald.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------*/

#include "ChaosEmerald.h"

//------------
// Constructor
//------------
ChaosEmerald::ChaosEmerald()
{
	m_texture = { 0, 0, 0, 0, 0 };
	m_position = { 0,0 };
}

//-----------------------
// Draw the Chaos Emerald
//-----------------------
void ChaosEmerald::Draw()
{
	DrawTexture(m_texture, (int)m_position.x, (int)m_position.y, WHITE);
}