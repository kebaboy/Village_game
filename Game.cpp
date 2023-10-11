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
    _resourceManager.LoadGameTexture("player", "chel.png");
    _resourceManager.LoadGameTexture("grass", "grass.png");
    _resourceManager.LoadGameTexture("sand", "sand.png");
    _resourceManager.LoadGameTexture("house", "house.png");
    _resourceManager.LoadGameTexture("start_btn", "start_btn.png");
    _resourceManager.LoadGameTexture("tree", "tree.png");
    _resourceManager.LoadGameTexture("rock", "rock.png");
    _player.SetTexture(_resourceManager.GetGameTexture("player"));
    _camera = {0};
    _camera.target = _player.GetPositon();
    _camera.offset = Vector2{(float)(_screenWidth/2), (float)_screenHeight/2};
    _camera.rotation = 0.0f;
    _camera.zoom = 1.0f;
    _ui.Initialize(Vector2 {(float)_screenWidth, (float)_screenHeight}, _resourceManager);
    _trees.push_back(Tree(Vector2{500, 600}, Vector2{100.0f, 180.0f},_resourceManager.GetGameTexture("tree")));
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
    _ui.UpdateAll();
//    if (_ui.GetElement(0)->BtnPressed()) {
//        this->ToggleHousePlacingMode();
//        std::cout << "house placing mode: " << _placingHouseMode << std::endl;
//    }
    if (static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(0)->BtnPressed()) {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            _buildings.push_back(Building(GetScreenToWorld2D(mousePos, _camera), Vector2{100, 100}, _resourceManager.GetGameTexture("house")));
            this->ToggleHousePlacingMode();
            static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(0)->BtnUnpressed(); // Закончить режим размещения
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
                    DrawTexturePro(_resourceManager.GetGameTexture("grass"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("grass").width, (float)_resourceManager.GetGameTexture("grass").height}, Rectangle{positionTile.x, positionTile.y, (float)_tileSize, (float)_tileSize}, Vector2{0,0}, 0.0f, WHITE);
                    break;
                case 1:
                    DrawRectangle((float)positionTile.x, (float)positionTile.y, _tileSize, _tileSize, BROWN);
                    break;
                case 3:
                    DrawTexturePro(_resourceManager.GetGameTexture("sand"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("sand").width, (float)_resourceManager.GetGameTexture("sand").height}, Rectangle{positionTile.x, positionTile.y, (float)_tileSize, (float)_tileSize}, Vector2{0,0}, 0.0f, WHITE);
            }
        }
    }
    for (const auto& house : _buildings) {
        house.Draw();
    }
    for (const auto& tree : _trees) {
        tree.Draw();
    }
    // Если мы в режиме размещения, рисуем контур дома

    _player.Draw();
    EndMode2D();
    if (static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(0)->BtnPressed()) {
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
