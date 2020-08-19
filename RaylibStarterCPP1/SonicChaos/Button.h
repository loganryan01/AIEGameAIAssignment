/*--------------------------------------------------------------------
	File Name: Button.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once

#include "raylib.h"
#include <string>
#include <functional>

class Button
{
public:
	// Constructor
	Button(std::string text, Rectangle shape);

	// Action to be taken on click
	void OnClick(std::function<void()> callback);

	// Update the button
	void Update();

	// Draw the button
	void Draw();

private:
	// End position x of the button shape
	int m_endPositionX;

	// End position x of the button shape
	int m_endPositionY;

	// Message to be displayed inside the button
	std::string m_text;

	// The shape of the button
	Rectangle m_shape;

	// The position of the button
	Vector2 m_mousePosition;

	// Function to be done when the button is clicked
	std::function<void()> m_onClick;
};

