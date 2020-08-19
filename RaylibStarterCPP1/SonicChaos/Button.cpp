#include "Button.h"

Button::Button(std::string text, Rectangle shape) : m_text(text), m_shape(shape), m_mousePosition(GetMousePosition()),
m_endPositionX(shape.x + shape.width), m_endPositionY(shape.y + shape.height)
{

}

// Callback Function
void Button::OnClick(std::function<void()> callback)
{
	m_onClick = callback;
}

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

void Button::Draw()
{
	DrawRectangleLines(m_shape.x, m_shape.y, m_shape.width, m_shape.height, BLACK);

	char* cstr = new char[m_text.length() + 1];
	std::strcpy(cstr, m_text.c_str());

	DrawText(cstr, m_shape.x + 5.0f, m_shape.y + 1.0f, 40, DARKBLUE);

	if (m_mousePosition.x >= m_shape.x && m_mousePosition.x <= m_endPositionX &&
		m_mousePosition.y >= m_shape.y && m_mousePosition.y <= m_endPositionY)
	{
		DrawRectangleLines(m_shape.x, m_shape.y, m_shape.width, m_shape.height, BLUE);
	}
}