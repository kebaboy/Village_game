//
// Created by Григорий on 05.10.2023.
//

#include "Game.h"

Game::Game() : _screenWidth(1000), _screenHeight(650),
                _tileSize(100),
                _player(Vector2{ (float)(_screenWidth/2), (float)_screenHeight/2})
                {}

void Game::Initialize() {
    InitWindow(_screenWidth, _screenHeight, "Village");
    _resourceManager.LoadGameTexture("playerSprite", "chel.png");
    _resourceManager.LoadGameTexture("grassSprite", "grass.png");
    _resourceManager.LoadGameTexture("houseSprite", "house.png");
    _player.SetTexture(_resourceManager.GetGameTexture("playerSprite"));
    SetTargetFPS(60);
}

void Game::Update() {
    _player.Update();
}

void Game::Draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            Vector2 positionTile = {(float)( -100 + x * _tileSize), (float)(-100 + y * _tileSize)};
            switch (_map[y][x]) {
                case 0:
                    DrawTexturePro(_resourceManager.GetGameTexture("grassSprite"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("grassSprite").width, (float)_resourceManager.GetGameTexture("grassSprite").height}, Rectangle{positionTile.x, positionTile.y, (float)_tileSize, (float)_tileSize}, Vector2{0,0}, 0.0f, WHITE);
                    break;
                case 1:
                    DrawRectangle((float)positionTile.x, (float)positionTile.y, _tileSize, _tileSize, BROWN);
            }
        }
    }
    _player.Draw();
    DrawFPS(20, 20);

    EndDrawing();
}

void Game::Run() {
    Initialize();
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    CloseWindow();
}
