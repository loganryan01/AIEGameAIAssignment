/*----------------------------
	File Name: Button.cpp
	Purpose: Create a button.
	Author: Logan Ryan
	Modified: 19 August 2020
------------------------------
	Copyright 2020 Logan Ryan.
----------------------------*/

#include "Button.h"

//--------------------------------------------------------------------------
// Constructor
//	text (std::string): What message should be in the button?
//	shape (Rectangle): What is the position and measurements for the button?
//--------------------------------------------------------------------------
Button::Button(std::string text, Rectangle shape) : m_text(text), m_shape(shape), m_mousePosition(GetMousePosition()),
m_endPositionX((int)shape.x + (int)shape.width), m_endPositionY((int)shape.y + (int)shape.height)
{

}

//-------------------------------------------------------------
// Action to be taken on click
//	callback (std::function<void()>): What is the button doing?
//-------------------------------------------------------------
void Button::OnClick(std::function<void()> callback)
{
	m_onClick = callback;
}

//------------------
// Update the button
//------------------
void Button::Update()
{
	// Update the mouse position
	m_mousePosition = GetMousePosition();

	// Call the "OnClick" callback if the mouse button was pressed
	// when the mouse is within the rectangle area
	if (m_mousePosition.x >= m_shape.x && m_mousePosition.x <= m_endPositionX &&
		m_mousePosition.y >= m_shape.y && m_mousePosition.y <= m_endPositionY &&
		IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		m_onClick();
	}
}

//----------------
// Draw the button
//----------------
void Button::Draw()
{
	// Draw the shape
	DrawRectangleLines((int)m_shape.x, (int)m_shape.y, (int)m_shape.width, (int)m_shape.height, BLACK);

	char* cstr = new char[m_text.length() + 1];
	std::strcpy(cstr, m_text.c_str());

	// Draw the text
	DrawText(cstr, (int)m_shape.x + 5, (int)m_shape.y + 1, 40, DARKBLUE);

	// If the mouse is hovering over the button, change the colour of the border
	if (m_mousePosition.x >= m_shape.x && m_mousePosition.x <= m_endPositionX &&
		m_mousePosition.y >= m_shape.y && m_mousePosition.y <= m_endPositionY)
	{
		DrawRectangleLines((int)m_shape.x, (int)m_shape.y, (int)m_shape.width, (int)m_shape.height, BLUE);
	}
}