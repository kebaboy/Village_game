//
// Created by Григорий on 05.10.2023.
//

#include "Game.h"
#include "iostream"

Game::Game() : _screenWidth(1000), _screenHeight(650),
                _map(40, 30, 100),
                _player(Vector2{ (float)(500/2), (float)(500/2)}, Vector2{float(50), float(50)})
                {}

void Game::Initialize() {
    InitWindow(_screenWidth, _screenHeight, "Village");
    _resourceManager.LoadGameTexture("player", "chel.png");
    _resourceManager.LoadGameTexture("grass", "grass.png");
    _resourceManager.LoadGameTexture("sand", "sand.png");
    _resourceManager.LoadGameTexture("house", "house.png");
    _resourceManager.LoadGameTexture("start_btn", "start_btn.png");
    _resourceManager.LoadGameTexture("tree", "tree.png");
    _resourceManager.LoadGameTexture("stone", "stone.png");
    _resourceManager.LoadGameTexture("lumberjack", "lumberjack.png");
    _resourceManager.LoadGameTexture("wood_storage", "wood-storage.png");
    _map.Generate();
    _player.SetTexture(_resourceManager.GetGameTexture("player"));
    _camera = {0};
    _camera.target = _player.GetPositon();
    _camera.offset = Vector2{(float)_screenWidth/2, (float)_screenHeight/2};
    _camera.rotation = 0.0f;
    _camera.zoom = 1.0f;
    _ui.Initialize(Vector2 {(float)_screenWidth, (float)_screenHeight}, _resourceManager);
//    _trees.push_back(Tree(Vector2{500, 600}, Vector2{100.0f, 180.0f},_resourceManager.GetGameTexture("tree")));
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
    BuildMenu* pbuildMenu = static_cast<BuildMenu*>(_ui.GetElement(1));
    if (pbuildMenu->GetBuildMenuElement(0)->BtnPressed()) {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            Vector2 worldPosition = GetScreenToWorld2D(mousePos, _camera);
            _lumberjackHouses.push_back(LumberjackHouse(worldPosition, _resourceManager.GetGameTexture("house")));
            this->ToggleHousePlacingMode();
            pbuildMenu->GetBuildMenuElement(0)->BtnUnpressed(); // Закончить режим размещения
            _lumberjasks.push_back(Lumberjack(Vector2 {worldPosition.x + 50, worldPosition.y + 50}, Vector2{float(80), float(50)}, _resourceManager.GetGameTexture("lumberjack")));
            _lumberjasks.back().SetTargetTreePosition(_map.FindClosestTreePositon(worldPosition));

            std::cout << "push ok\n";
        }
    }
    if (pbuildMenu->GetBuildMenuElement(1)->BtnPressed()) {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            Vector2 worldPosition = GetScreenToWorld2D(mousePos, _camera);
            _woodStorages.push_back(WoodStorage(worldPosition, _resourceManager.GetGameTexture("wood_storage")));
            this->ToggleHousePlacingMode();
            pbuildMenu->GetBuildMenuElement(1)->BtnUnpressed(); // Закончить режим размещения
            std::cout << "push ok\n";
        }
    }
    for (auto& lumberjack : _lumberjasks) {
        lumberjack.Update(_woodCount);
    }
}

void Game::Draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(_camera);
//    for (int y = 0; y < 20; y++) {
//        for (int x = 0; x < 20; x++) {
//            Vector2 positionTile = {(float)(x * _tileSize), (float)(y * _tileSize)};
//            switch (_map[y][x]) {
//                case 0:
//                    DrawTexturePro(_resourceManager.GetGameTexture("grass"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("grass").width, (float)_resourceManager.GetGameTexture("grass").height}, Rectangle{positionTile.x, positionTile.y, (float)_tileSize, (float)_tileSize}, Vector2{0,0}, 0.0f, WHITE);
//                    break;
//                case 1:
//                    DrawRectangle((float)positionTile.x, (float)positionTile.y, _tileSize, _tileSize, BROWN);
//                    break;
//                case 3:
//                    DrawTexturePro(_resourceManager.GetGameTexture("sand"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("sand").width, (float)_resourceManager.GetGameTexture("sand").height}, Rectangle{positionTile.x, positionTile.y, (float)_tileSize, (float)_tileSize}, Vector2{0,0}, 0.0f, WHITE);
//            }
//        }
//    }
    _map.Draw(_resourceManager);
    for (const auto& house : _lumberjackHouses) {
        house.Draw();
    }
    for (const auto& storage : _woodStorages) {
        storage.Draw();
    }
    for (const auto& lumberjack : _lumberjasks) {
        lumberjack.Draw();
    }
    for (const auto& tree : _trees) {
        tree.Draw();
    }
    // Если мы в режиме размещения, рисуем контур дома

    _player.Draw();
    EndMode2D();
    if (static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(0)->BtnPressed()) {
        DrawRectangleLines((float)GetMousePosition().x, (float)GetMousePosition().y, 100, 100, BLACK);
        DrawTexturePro(static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(0)->GetTexture(), Rectangle{0.0f, 0.0f, (float)static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(0)->GetTexture().width, (float)static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(0)->GetTexture().height}, Rectangle{(float)GetMousePosition().x, (float)GetMousePosition().y, 100.0f, 100.0}, Vector2{0,0}, 0.0f, Color{ 255, 255, 255, 128 });

    }
    if (static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(1)->BtnPressed()) {
        DrawRectangleLines((float)GetMousePosition().x, (float)GetMousePosition().y, 150, 150, BLACK);
        DrawTexturePro(static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(1)->GetTexture(), Rectangle{0.0f, 0.0f, (float)static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(1)->GetTexture().width, (float)static_cast<BuildMenu*>(_ui.GetElement(1))->GetBuildMenuElement(1)->GetTexture().height}, Rectangle{(float)GetMousePosition().x, (float)GetMousePosition().y, 150.0f, 150.0}, Vector2{0,0}, 0.0f, Color{ 255, 255, 255, 128 });

    }
    _ui.DrawAll();
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Wood: %d", _woodCount);
    DrawText(buffer, 20, 20, 20, BLACK);
    DrawFPS(20, 50);

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

//Vector2 Game::GetMapSize() const {
//    return _mapSize;
//}

void Game::PlayerMove(Vector2 direction) {
    Vector2 newPosition = {_player.GetPositon().x + direction.x, _player.GetPositon().y + direction.y};
    if (newPosition.x >= 0 && newPosition.x < _map.GetMapSize().x - _player.GetSize().x &&
        newPosition.y >= 0 && newPosition.y < _map.GetMapSize().y - _player.GetSize().y) {
        _player.SetPosition(newPosition);
    }
}

void Game::UpdateCamera() {
    _camera.target = _player.GetPositon();

    if (_camera.target.x - (_screenWidth / 2) < 0) _camera.target.x = _screenWidth / 2;
    if (_camera.target.y - (_screenHeight / 2) < 0) _camera.target.y = _screenHeight / 2;
    if (_camera.target.x + (_screenWidth / 2) > _map.GetMapSize().x) _camera.target.x = _map.GetMapSize().x - (_screenWidth / 2);
    if (_camera.target.y + (_screenHeight / 2) > _map.GetMapSize().y) _camera.target.y = _map.GetMapSize().y - (_screenHeight / 2);
}

void Game::ToggleHousePlacingMode() {
    _placingHouseMode = !_placingHouseMode;
}

bool Game::IsInHousePlacingMode() const {
    return _placingHouseMode;
}
