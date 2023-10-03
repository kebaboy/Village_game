#include <iostream>
#include "raylib.h"

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 650;
    int map[10][10] = {
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    };
    int tileSize = 100;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    Texture2D man = LoadTexture("../chel.png");
    Texture2D grass = LoadTexture("../grass.png");
    Texture2D house = LoadTexture("../house.png");

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

    Vector2 mousePoint = {0.0f, 0.0f};
    Rectangle button = {screenWidth - 100.0f, screenHeight - 100.0f, 80, 80};

    Rectangle houseRec = {400, 200, 80, 80};
    bool textureDrawn = false;

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
            position.x += 2.0f;
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
            position.x -= 2.0f;
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
            position.y -= 2.0f;
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
            position.y += 2.0f;
        }
        camera.target = position;

        mousePoint = GetMousePosition();
        Vector2 mouseWorldPoint = GetScreenToWorld2D(mousePoint,  camera);
        bool colisn = CheckCollisionPointRec(mouseWorldPoint, houseRec);
        bool pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        if (colisn && pressed && !textureDrawn) {
            textureDrawn = true;
        }




        BeginDrawing();
        ClearBackground(RAYWHITE);
//        for (float x = (float)screenWidth/2 - position.x; x < screenWidth - 500; x += 200) {
//            for (float y = (float)screenHeight/2 - position.y; y < screenHeight - 500; y += 200) {
//                DrawTexturePro(grass, Rectangle{0.0f, 0.0f, (float)grass.width, (float)grass.height}, Rectangle{(float)x, (float)y, 200, 200}, Vector2{0,0}, 0.0f, WHITE);
//            }
//        }
        BeginMode2D(camera);
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                Vector2 positionTile = {(float)( -100 + x * tileSize), (float)(-100 + y * tileSize)};
                switch (map[y][x]) {
                    case 0:
                        DrawTexturePro(grass, Rectangle{0.0f, 0.0f, (float)grass.width, (float)grass.height}, Rectangle{positionTile.x, positionTile.y, (float)tileSize, (float)tileSize}, Vector2{0,0}, 0.0f, WHITE);
                        break;
                    case 1:
                        DrawRectangle((float)positionTile.x, (float)positionTile.y, tileSize, tileSize, BROWN);
                }
            }
        }
        if (textureDrawn == false) DrawRectangle(400, 200, 80, 80, RED);
        else DrawTexturePro(house, Rectangle{0.0f, 0.0f, (float)house.width, (float)house.height}, Rectangle{houseRec.x - 60, houseRec.y - 100, houseRec.width + 100, houseRec.height + 100}, Vector2{0,0}, 0.0f, WHITE);
        DrawTexturePro(man, frameRec, Rectangle{position.x,position.y, 70, 70}, Vector2{0,0}, 0.0f, WHITE);

        EndMode2D();
        DrawRectangleRec(button, BROWN);
        DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadTexture(man);
    UnloadTexture(grass);
    UnloadTexture(house);
    CloseWindow();
    return 0;
}
