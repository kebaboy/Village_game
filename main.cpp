#include <iostream>
#include "raylib.h"

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    Texture2D man = LoadTexture("../chel.png");

    auto frameWidth = (float)(man.width / 4);
    auto frameHeight = (float)(man.height / 4);
    Vector2 position = { (float)(screenWidth/2), (float)screenHeight/2};
    Rectangle frameRec = {0.0f, 0.0f, frameWidth, frameHeight};
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 4;
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_RIGHT)) {
            if (frameRec.y != frameHeight) frameRec.y = frameHeight;
            if (!IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN) and !IsKeyDown(KEY_LEFT)) framesCounter++;
            if (framesCounter >= (60/framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 3) currentFrame = 0;

                frameRec.x = (float)currentFrame*frameWidth;
            }
            position.x += 3.0f;
        }
        if (IsKeyDown(KEY_LEFT)) {
            if (frameRec.y != frameHeight * 2) frameRec.y = frameHeight * 2;
            if (!IsKeyDown(KEY_UP) and !IsKeyDown(KEY_DOWN) and !IsKeyDown(KEY_RIGHT)) framesCounter++;
            if (framesCounter >= (60/framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 3) currentFrame = 0;

                frameRec.x = (float)currentFrame*frameWidth;
            }
            position.x -= 3.0f;
        }
        if (IsKeyDown(KEY_UP)) {
            if (frameRec.y != frameHeight * 3) frameRec.y = frameHeight * 3;
            framesCounter++;
            if (framesCounter >= (60/framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 3) currentFrame = 0;

                frameRec.x = (float)currentFrame*frameWidth;
            }
            position.y -= 3.0f;
        }
        if (IsKeyDown(KEY_DOWN)) {
            if (frameRec.y != 0.0f) frameRec.y = 0.0f;
            if (!IsKeyDown(KEY_UP)) framesCounter++;
            if (framesCounter >= (60/framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 3) currentFrame = 0;

                frameRec.x = (float)currentFrame*frameWidth;
            }
            position.y += 3.0f;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);
        DrawTexturePro(man, frameRec, Rectangle{position.x,position.y, 100, 100}, Vector2{0,0}, 0.0f, WHITE);
        EndDrawing();
    }
    UnloadTexture(man);
    CloseWindow();
    return 0;
}
