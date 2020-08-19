/*-----------------------------------------
	File Name: main.cpp
	Purpose: Start and end the application.
	Author: Logan Ryan
	Modified: 19 August 2020
-------------------------------------------
	Copyright 2020 Logan Ryan.
-----------------------------------------*/

#include "raylib.h"
#include "Application.h"

int main(int argc, char* argv[])
{
    {
        Application app(800, 800);
        app.Run();
    }

    return 0;
}