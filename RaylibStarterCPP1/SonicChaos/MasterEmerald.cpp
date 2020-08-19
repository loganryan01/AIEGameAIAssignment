/*---------------------------------
	File Name: MasterEmerald.cpp
	Purpose: Draw a Master Emerald.
	Author: Logan Ryan
	Modified: 19 August 2020
-----------------------------------
	Copyright 2020 Logan Ryan.
---------------------------------*/

#include "MasterEmerald.h"

//------------
// Constructor
//------------
MasterEmerald::MasterEmerald()
{
	m_texture = { 0, 0, 0, 0, 0 };
	m_position = { 0,0 };
}

//------------------------
// Draw the Master Emerald
//------------------------
void MasterEmerald::Draw()
{
	DrawTexture(m_texture, (int)m_position.x, (int)m_position.y, WHITE);
}