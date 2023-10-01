#include <iostream>
#include "raylib.h"

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 650;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    Texture2D man = LoadTexture("../chel.png");
    Texture2D grass = LoadTexture("../grass.png");

    auto frameWidth = (float)(man.width / 4);
    auto frameHeight = (float)(man.height / 4);
    Vector2 position = { (float)(screenWidth/2), (float)screenHeight/2};
    Rectangle frameRec = {0.0f, 0.0f, frameWidth, frameHeight};
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 4;

    Camera2D camera = {0};
    camera.target = position;
    camera.offset = Vector2{(float)(screenWidth/2), (float)screenHeight/2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
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
        camera.target = position;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (float x = (float)screenWidth/2 - position.x; x < screenWidth; x += 200) {
            for (float y = (float)screenHeight/2 - position.y; y < screenHeight; y += 200) {
                DrawTexturePro(grass, Rectangle{0.0f, 0.0f, (float)grass.width, (float)grass.height}, Rectangle{(float)x, (float)y, 200, 200}, Vector2{0,0}, 0.0f, WHITE);
            }
        }
        DrawFPS(10, 10);
        DrawRectangle(screenWidth/2.0f+500 - position.x, screenHeight/2.0f+500 - position.y, 80, 80, RED);
        BeginMode2D(camera);
        DrawTexturePro(man, frameRec, Rectangle{position.x,position.y, 70, 70}, Vector2{0,0}, 0.0f, WHITE);
        EndMode2D();
        EndDrawing();
    }
    UnloadTexture(man);
    UnloadTexture(grass);
    CloseWindow();
    return 0;
}
