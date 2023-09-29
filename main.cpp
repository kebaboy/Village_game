#include <iostream>
#include "raylib.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    Vector2 position = { (float)screenWidth/2, (float)screenHeight/2 };
    SetTargetFPS(60);
    Texture2D man = LoadTexture("../man.png");


    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT)) position.x += 4.0f;
        if (IsKeyDown(KEY_LEFT)) position.x -= 4.0f;
        if (IsKeyDown(KEY_UP)) position.y -= 4.0f;
        if (IsKeyDown(KEY_DOWN)) position.y += 4.0f;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);
        DrawTextureEx(man, position, 0, 0.1, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
