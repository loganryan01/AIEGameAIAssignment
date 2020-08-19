#pragma once

#include "raylib.h"
#include <string>
#include <functional>

class Button
{
public:
	Button(std::string text, Rectangle shape);

	void OnClick(std::function<void()> callback);
	void Update();
	void Draw();
private:
	int m_endPositionX;
	int m_endPositionY;

	std::string m_text;

	Rectangle m_shape;

	Vector2 m_mousePosition;

	std::function<void()> m_onClick;
};

