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
    _resourceManager.LoadGameTexture("lumberjack_house", "lmbrjck-house.png");
    _resourceManager.LoadGameTexture("miner_house", "miner-house.png");
    _resourceManager.LoadGameTexture("storage_category_button", "storage_category_button.png");
    _resourceManager.LoadGameTexture("house_category_button", "house_category_button.png");
    _resourceManager.LoadGameTexture("tree", "tree.png");
    _resourceManager.LoadGameTexture("stone", "stone.png");
    _resourceManager.LoadGameTexture("lumberjack", "lumberjack.png");
    _resourceManager.LoadGameTexture("miner", "miner.png");
    _resourceManager.LoadGameTexture("wood_storage", "wood-storage.png");
    _resourceManager.LoadGameTexture("stone_storage", "stone-storage.png");
    _resourceManager.LoadGameTexture("farm", "farm.png");
    _resourceManager.LoadGameTexture("build_menu_button", "build_menu_button.png");
    _resourceManager.LoadGameTexture("close_button", "close_button.png");
    _map.Generate();
    _player.SetTexture(_resourceManager.GetGameTexture("player"));
    _camera = {0};
    _camera.target = _player.GetPosition();
    _camera.offset = Vector2{(float)_screenWidth/2, (float)_screenHeight/2};
    _camera.rotation = 0.0f;
    _camera.zoom = 1.0f;
    _ui.Initialize(Vector2 {(float)_screenWidth, (float)_screenHeight}, _resourceManager, this);
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
    _ui.Update();
    if (_buildingPlacingMode) {
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            PlaceBuilding(GetMousePosition());
        }
    }

    std::vector<Storage*> woodStorages;
    for (auto& ws : _woodStorages) {
        woodStorages.push_back(&ws);
    }
    for (auto& lumberjack : _lumberjasks) {
        lumberjack.Update(woodStorages, _map);
    }
    std::vector<Storage*> stoneStorages;
    for (auto& ws : _stoneStorages) {
        stoneStorages.push_back(&ws);
    }
    for (auto& miner : _miners) {
        miner.Update(stoneStorages, _map);
    }
    _woodCounter = CalculateTotalWood();
    _stoneCounter = CalculateTotalStone();
}

void Game::Draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(_camera);
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
    for (const auto& house : _minerHouses) {
        house.Draw();
    }
    for (const auto& storage : _stoneStorages) {
        storage.Draw();
    }
    for (const auto& miner : _miners) {
        miner.Draw();
    }
    for (const auto& tree : _trees) {
        tree.Draw();
    }
    // Если мы в режиме размещения, рисуем контур дома

    _player.Draw();
    EndMode2D();
    if (_buildingPlacingMode) {
        switch (_previewBuildingType) {
            case BuildingType::WoodStorage:
                DrawTexturePro(_resourceManager.GetGameTexture("wood_storage"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("wood_storage").width, (float)_resourceManager.GetGameTexture("wood_storage").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 150.0f, 150.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
            case BuildingType::StoneStorage:
                DrawTexturePro(_resourceManager.GetGameTexture("stone_storage"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("stone_storage").width, (float)_resourceManager.GetGameTexture("stone_storage").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 150.0f, 150.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
            case BuildingType::LumberjackHouse:
                DrawTexturePro(_resourceManager.GetGameTexture("lumberjack_house"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("lumberjack_house").width, (float)_resourceManager.GetGameTexture("lumberjack_house").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 100.0f, 100.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
            case BuildingType::MinerHouse:
                DrawTexturePro(_resourceManager.GetGameTexture("miner_house"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("miner_house").width, (float)_resourceManager.GetGameTexture("miner_house").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 100.0f, 100.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
        }
    }
    _ui.Draw();
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Wood: %d / %d\nStone: %d / %d", (int)_woodCounter.x, (int)_woodCounter.y, (int)_stoneCounter.x, (int)_stoneCounter.y);
    DrawText(buffer, 20, 20, 20, WHITE);
    DrawFPS(20, 80);

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

void Game::PlaceBuilding(Vector2 position) {
    if (!_buildingPlacingMode) return;
    Vector2 worldPos = GetScreenToWorld2D(position, _camera);
    switch(_previewBuildingType) {
        case BuildingType::WoodStorage:
            _woodStorages.push_back(WoodStorage(worldPos, _resourceManager.GetGameTexture("wood_storage")));
            break;
        case BuildingType::StoneStorage:
            _stoneStorages.push_back(StoneStorage(worldPos, _resourceManager.GetGameTexture("stone_storage")));
            break;
//        case BuildingType::Farm:
        case BuildingType::LumberjackHouse:
            _lumberjackHouses.push_back(LumberjackHouse(worldPos, _resourceManager.GetGameTexture("lumberjack_house")));
            _lumberjasks.push_back(Lumberjack(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("lumberjack"), worldPos));
            _lumberjasks.back().SetHomePosition(worldPos);
            break;
        case BuildingType::MinerHouse:
            _minerHouses.push_back(MinerHouse(worldPos, _resourceManager.GetGameTexture("miner_house")));
            _miners.push_back(Miner(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("miner"), worldPos));
            _miners.back().SetHomePosition(worldPos);
            break;

    }
    _buildingPlacingMode = false;
}
void Game::PlayerMove(Vector2 direction) {
    Vector2 newPosition = {_player.GetPosition().x + direction.x, _player.GetPosition().y + direction.y};
    if (newPosition.x >= 0 && newPosition.x < _map.GetMapSize().x - _player.GetSize().x &&
        newPosition.y >= 0 && newPosition.y < _map.GetMapSize().y - _player.GetSize().y) {
        _player.SetPosition(newPosition);
    }
}

void Game::UpdateCamera() {
    _camera.target = _player.GetPosition();

    if (_camera.target.x - (_screenWidth / 2) < 0) _camera.target.x = _screenWidth / 2;
    if (_camera.target.y - (_screenHeight / 2) < 0) _camera.target.y = _screenHeight / 2;
    if (_camera.target.x + (_screenWidth / 2) > _map.GetMapSize().x) _camera.target.x = _map.GetMapSize().x - (_screenWidth / 2);
    if (_camera.target.y + (_screenHeight / 2) > _map.GetMapSize().y) _camera.target.y = _map.GetMapSize().y - (_screenHeight / 2);
}

void Game::ToggleBuildingPlacementMode(BuildingType type) {
    if (_buildingPlacingMode && _previewBuildingType == type) {
        _buildingPlacingMode = false;
    } else {
        _buildingPlacingMode = true;
        _previewBuildingType = type;
    }
}

Vector2 Game::CalculateTotalWood() {
    Vector2 woodCounter = {0.0f, 0.0f};
    if (_woodStorages.empty()) return woodCounter;
    for (auto& storage : _woodStorages) {
        woodCounter.x += storage.GetCurrentResourceCount();
        woodCounter.y += storage.GetCapacity();
    }
    return woodCounter;
}
Vector2 Game::CalculateTotalStone() {
    Vector2 stoneCounter = {0.0f, 0.0f};
    if (_stoneStorages.empty()) return stoneCounter;
    for (auto& storage : _stoneStorages) {
        stoneCounter.x += storage.GetCurrentResourceCount();
        stoneCounter.y += storage.GetCapacity();
    }
    return stoneCounter;
}
