//
// Created by Григорий on 05.10.2023.
//

#include "Game.h"
#include "iostream"

Game::Game() : _screenWidth(1000), _screenHeight(650),
                _tileSize(100),
                _player(Vector2{ (float)(_mapSize.x/2), (float)(_mapSize.y - 60)}, Vector2{float(50), float(50)})
                {}

void Game::Initialize() {
    InitWindow(_screenWidth, _screenHeight, "Village");
    _resourceManager.LoadGameTexture("playerSprite", "chel.png");
    _resourceManager.LoadGameTexture("grassSprite", "grass.png");
    _resourceManager.LoadGameTexture("sandSprite", "sand.png");
    _resourceManager.LoadGameTexture("houseSprite", "house.png");
    _player.SetTexture(_resourceManager.GetGameTexture("playerSprite"));
    _camera = {0};
    _camera.target = _player.GetPositon();
    _camera.offset = Vector2{(float)(_screenWidth/2), (float)_screenHeight/2};
    _camera.rotation = 0.0f;
    _camera.zoom = 1.0f;
    _ui.AddElement(std::make_unique<UIButton>(_screenWidth -100, _screenHeight - 100, 70, 70, "Click me!"));
    SetTargetFPS(60);
}

void Game::Update() {
    float plVelocity = _player.GetVelocity();
    if (IsKeyDown(KEY_RIGHT)) {
        PlayerMove({plVelocity, 0});
    }
    if (IsKeyDown(KEY_LEFT)) {
        PlayerMove({-plVelocity, 0});
    }
    if (IsKeyDown(KEY_UP)) {
        PlayerMove({0, -plVelocity});
    }
    if (IsKeyDown(KEY_DOWN)) {
        PlayerMove({0, plVelocity});
    }
    UpdateCamera();
    if (_ui.BtnPressed()) {
        this->ToggleHousePlacingMode();
        std::cout << "house placing mode: " << _placingHouseMode << std::endl;
    }
    if (_placingHouseMode) {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
//            worldPosition.x = mousePos.x / _camera.zoom + _camera.target.x - (GetScreenWidth() * 0.5f);
//            worldPosition.y = mousePos.y / _camera.zoom + _camera.target.y - (GetScreenHeight() * 0.5f);
            _houses.push_back(House(Vector2 {mousePos.x / _camera.zoom + _camera.target.x - (_screenWidth * 0.5f), mousePos.y / _camera.zoom + _camera.target.y - (_screenHeight * 0.5f)}, Vector2{100, 100}, _resourceManager.GetGameTexture("houseSprite")));
            _placingHouseMode = false; // Закончить режим размещения
            std::cout << "push ok\n";
        }
    }
}

void Game::Draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(_camera);
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            Vector2 positionTile = {(float)(x * _tileSize), (float)(y * _tileSize)};
            switch (_map[y][x]) {
                case 0:
                    DrawTexturePro(_resourceManager.GetGameTexture("grassSprite"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("grassSprite").width, (float)_resourceManager.GetGameTexture("grassSprite").height}, Rectangle{positionTile.x, positionTile.y, (float)_tileSize, (float)_tileSize}, Vector2{0,0}, 0.0f, WHITE);
                    break;
                case 1:
                    DrawRectangle((float)positionTile.x, (float)positionTile.y, _tileSize, _tileSize, BROWN);
                    break;
                case 3:
                    DrawTexturePro(_resourceManager.GetGameTexture("sandSprite"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("sandSprite").width, (float)_resourceManager.GetGameTexture("sandSprite").height}, Rectangle{positionTile.x, positionTile.y, (float)_tileSize, (float)_tileSize}, Vector2{0,0}, 0.0f, WHITE);
            }
        }
    }
    for (const auto& house : _houses) {
        house.Draw();
    }
    // Если мы в режиме размещения, рисуем контур дома

    _player.Draw();
    EndMode2D();
    if (_placingHouseMode) {
        DrawRectangleLines((float)GetMousePosition().x, (float)GetMousePosition().y, 100, 100, BLACK);
    }
    _ui.DrawAll();
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

Vector2 Game::GetMapSize() const {
    return _mapSize;
}

void Game::PlayerMove(Vector2 direction) {
    Vector2 newPosition = {_player.GetPositon().x + direction.x, _player.GetPositon().y + direction.y};
    if (newPosition.x >= 0 && newPosition.x < _mapSize.x - _player.GetSize().x &&
        newPosition.y >= 0 && newPosition.y < _mapSize.y - _player.GetSize().y) {
        _player.SetPosition(newPosition);
    }
}

void Game::UpdateCamera() {
    _camera.target = _player.GetPositon();

    if (_camera.target.x - (_screenWidth / 2) < 0) _camera.target.x = _screenWidth / 2;
    if (_camera.target.y - (_screenHeight / 2) < 0) _camera.target.y = _screenHeight / 2;
    if (_camera.target.x + (_screenWidth / 2) > _mapSize.x) _camera.target.x = _mapSize.x - (_screenWidth / 2);
    if (_camera.target.y + (_screenHeight / 2) > _mapSize.y) _camera.target.y = _mapSize.y - (_screenHeight / 2);
}

void Game::ToggleHousePlacingMode() {
    _placingHouseMode = !_placingHouseMode;
}

bool Game::IsInHousePlacingMode() const {
    return _placingHouseMode;
}
