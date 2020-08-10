/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "Application.h"

struct Sonic
{
    float x;
    float y;
    Texture2D texture;
};

int main(int argc, char* argv[])
{
    {
        Application app(800, 800);
        app.Run();
    }
    
    // Initialization
    //--------------------------------------------------------------------------------------
    float screenWidth = 800;
    float screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Sonic Chaos");

    SetTargetFPS(60);

    Sonic sonic;

    Texture2D map = LoadTexture("C:/Users/logan/Desktop/AIEGameAIAssignment/RaylibStarterCPP1/resources/tilemap.png");
    sonic.texture = LoadTexture("C:/Users/logan/Desktop/AIEGameAIAssignment/RaylibStarterCPP1/resources/Sonic.png");
    sonic.x = 16;
    sonic.y = 32;
    Camera2D camera = { 0 };
    camera.target = { sonic.x + sonic.texture.width, sonic.y + sonic.texture.height };
    camera.offset = { screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawTexture(map, 0, 0, WHITE);
        DrawTexture(sonic.texture, sonic.x, sonic.y, WHITE);

        EndMode2D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}