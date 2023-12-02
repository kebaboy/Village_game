//
// Created by Григорий on 05.10.2023.
//

#include "Game.h"
#include "iostream"

Game::Game() : _screenWidth(1000), _screenHeight(650),
                _currentState(GameState::MainMenu),
                _menu(Menu(_screenWidth, _screenHeight)),
                _map(40, 30, 100),
                _player(Vector2{ (float)(500/2), (float)(500/2)}, Vector2{float(50), float(50)}),
                _townhall(Vector2{700.0f, 700.0f})
                {}

void Game::Run() {
    Initialize();
    while (!WindowShouldClose()) {
        Update();
        Render();
    }
    CloseWindow();
}

void Game::Initialize() {
    InitWindow(_screenWidth, _screenHeight, "Village");
    _resourceManager.LoadGameTexture("player", "chel.png");
    _resourceManager.LoadGameTexture("grass", "grass.png");
    _resourceManager.LoadGameTexture("sand", "sand.png");
    _resourceManager.LoadGameTexture("lumberjack_house", "lmbrjck-house.png");
    _resourceManager.LoadGameTexture("miner_house", "miner-house.png");
    _resourceManager.LoadGameTexture("farmer_house", "farmer-house.png");
    _resourceManager.LoadGameTexture("storage_category_button", "storage_category_button.png");
    _resourceManager.LoadGameTexture("house_category_button", "house_category_button.png");
    _resourceManager.LoadGameTexture("defence_category_button", "defence_category_button.png");
    _resourceManager.LoadGameTexture("tree", "tree.png");
    _resourceManager.LoadGameTexture("stone", "stone.png");
    _resourceManager.LoadGameTexture("lumberjack", "lumberjack.png");
    _resourceManager.LoadGameTexture("miner", "miner.png");
    _resourceManager.LoadGameTexture("farmer", "farmer.png");
    _resourceManager.LoadGameTexture("wood_storage", "wood-storage.png");
    _resourceManager.LoadGameTexture("stone_storage", "stone-storage.png");
    _resourceManager.LoadGameTexture("farm", "farm.png");
    _resourceManager.LoadGameTexture("build_menu_button", "build_menu_button.png");
    _resourceManager.LoadGameTexture("close_button", "close_button.png");
    _resourceManager.LoadGameTexture("barrack", "barrack.png");
    _resourceManager.LoadGameTexture("knight", "knight.png");
    _resourceManager.LoadGameTexture("raider", "raider.png");
    _resourceManager.LoadGameTexture("townhall", "townhall.png");
    _resourceManager.LoadGameTexture("sleep", "sleep.png");
    _resourceManager.LoadGameTexture("start", "start_btn.png");
    _menu.Initialize(_resourceManager);
    _map.Generate();
    _player.SetTexture(_resourceManager.GetGameTexture("player"));
    _townhall.SetTexture(_resourceManager.GetGameTexture("townhall"));
    _camera = {0};
    _camera.target = _player.GetPosition();
    _camera.offset = Vector2{(float)_screenWidth/2, (float)_screenHeight/2};
    _camera.rotation = 0.0f;
    _camera.zoom = 1.0f;
    _ui.Initialize(Vector2 {(float)_screenWidth, (float)_screenHeight}, _resourceManager, this);
    SetTargetFPS(60);
}

void Game::Update() {
    HandleStates();
}

void Game::HandleStates() {
    switch (_currentState) {
        case GameState::MainMenu:
            HandleMenu();
            break;
        case GameState::InGame:
            HandleGame();
            break;
    }
}

void Game::HandleMenu() {
    _menu.Update();
    if (_menu.GetCurrentState() == MenuState::StartGame) {
        _currentState = GameState::InGame;
        _menu.Reset();
    }
}

void Game::HandleGame() {
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

    if (IsKeyPressed(KEY_R) && !_isRaidActive) {
        StartRaidEvent();
    }
    if (_isRaidActive) {
        HandleRaid();
    }

    std::vector<Storage*> woodStorages;
    for (auto& ws : _woodStorages) {
        woodStorages.push_back(&ws);
    }
    for (auto& lumberjack : _lumberjacks) {
        lumberjack.Update(woodStorages, _map);
    }
    std::vector<Storage*> stoneStorages;
    for (auto& ws : _stoneStorages) {
        stoneStorages.push_back(&ws);
    }
    std::vector<Storage*> farms;
    for (auto& ws : _farms) {
        farms.push_back(&ws);
    }
    std::vector<Storage*> barracks;
    for (auto& ws : _barracks) {
        barracks.push_back(&ws);
    }
    for (auto& miner : _miners) {
        miner.Update(stoneStorages, _map);
    }
    for (auto& farmer: _farmers) {
        farmer.Update(_farms, _map);
    }

    for (auto& barrack: _barracks) {
        barrack.Update(_resourceManager, _camera, _raiders, _isRaidActive);
    }
    if (_isRaidActive) {
        std::vector<GameObject*> allTargets;
        allTargets.insert(allTargets.end(), woodStorages.begin(), woodStorages.end());
        allTargets.insert(allTargets.end(), stoneStorages.begin(), stoneStorages.end());
        allTargets.insert(allTargets.end(), farms.begin(), farms.end());
        allTargets.push_back(&_townhall);
        allTargets.erase(
                std::remove_if(allTargets.begin(), allTargets.end(), [](GameObject* obj) {return dynamic_cast<Storage*>(obj)->IsDestroyed();}),
                allTargets.end()
        );
        for (auto& raider : _raiders) {
            raider.Update(allTargets, _map);
        }
    }

    _woodCounter = CalculateTotalWood();
    _stoneCounter = CalculateTotalStone();
    _foodCounter = CalculateTotalFood();

    if (_townhall.IsDestroyed()) {
        Reset();
    }
}

void Game::Render() {
    switch (_currentState) {
        case GameState::MainMenu:
            _menu.Draw();
            break;
        case GameState::InGame:
            Draw();
            break;
    }
}

void Game::Draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode2D(_camera);
    _map.Draw(_resourceManager);
    _townhall.Draw();
    for (const auto& house : _lumberjackHouses) {
        house.Draw();
    }
    for (const auto& house : _minerHouses) {
        house.Draw();
    }
    for (const auto& house : _farmerHouses) {
        house.Draw();
    }

    for (const auto& storage : _woodStorages) {
        storage.Draw();
    }
    for (const auto& storage : _stoneStorages) {
        storage.Draw();
    }
    for (const auto& farm : _farms) {
        farm.Draw();
    }
    for (const auto& barrack : _barracks) {
        barrack.Draw();
    }

    for (const auto& worker : _lumberjacks) {
        worker.Draw();
    }
    for (const auto& worker : _miners) {
        worker.Draw();
    }
    for (const auto& worker : _farmers) {
        worker.Draw();
    }
    for (const auto& raider : _raiders) {
        if (raider.IsAlive()) raider.Draw();
    }
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
            case BuildingType::Farm:
                DrawTexturePro(_resourceManager.GetGameTexture("farm"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("farm").width, (float)_resourceManager.GetGameTexture("farm").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 150.0f, 150.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
            case BuildingType::LumberjackHouse:
                DrawTexturePro(_resourceManager.GetGameTexture("lumberjack_house"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("lumberjack_house").width, (float)_resourceManager.GetGameTexture("lumberjack_house").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 100.0f, 100.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
            case BuildingType::MinerHouse:
                DrawTexturePro(_resourceManager.GetGameTexture("miner_house"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("miner_house").width, (float)_resourceManager.GetGameTexture("miner_house").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 100.0f, 100.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
            case BuildingType::FarmerHouse:
                DrawTexturePro(_resourceManager.GetGameTexture("farmer_house"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("farmer_house").width, (float)_resourceManager.GetGameTexture("farmer_house").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 100.0f, 100.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
            case BuildingType::Barrack:
                DrawTexturePro(_resourceManager.GetGameTexture("barrack"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("barrack").width, (float)_resourceManager.GetGameTexture("barrack").height}, Rectangle{GetMousePosition().x, GetMousePosition().y, 150.0f, 150.0f}, Vector2{0, 0}, 0.0f, Color{255, 255, 255, 128 });
                break;
        }
    }
    _ui.Draw();
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Wood: %d / %d\nStone: %d / %d\nFood: %d / %d", (int)_woodCounter.x, (int)_woodCounter.y, (int)_stoneCounter.x, (int)_stoneCounter.y, (int)_foodCounter.x, (int)_foodCounter.y);
    if (_isRaidActive) {
        char buffer2[20];
        snprintf(buffer2, sizeof(buffer2), "Raiders: %d", (int)_raiders.size());
        DrawText(buffer2, _screenWidth / 2 - 70, 20, 40, RED);
    }
    DrawText(buffer, 20, 20, 20, WHITE);
    DrawFPS(20, 110);

    EndDrawing();
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
        case BuildingType::Farm:
            _farms.push_back(Farm(worldPos, _resourceManager.GetGameTexture("farm")));
            break;
        case BuildingType::LumberjackHouse:
            _lumberjackHouses.push_back(LumberjackHouse(worldPos, _resourceManager.GetGameTexture("lumberjack_house")));
            _lumberjacks.push_back(Lumberjack(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("lumberjack"), worldPos));
            _lumberjacks.back().SetHomePosition(worldPos);
            break;
        case BuildingType::MinerHouse:
            _minerHouses.push_back(MinerHouse(worldPos, _resourceManager.GetGameTexture("miner_house")));
            _miners.push_back(Miner(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("miner"), worldPos));
            _miners.back().SetHomePosition(worldPos);
            break;
        case BuildingType::FarmerHouse:
            _farmerHouses.push_back(FarmerHouse(worldPos, _resourceManager.GetGameTexture("farmer_house")));
            _farmers.push_back(Farmer(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("farmer"), worldPos));
            _farmers.back().SetHomePosition(worldPos);
            break;
        case BuildingType::Barrack:
            _barracks.push_back(Barrack(worldPos, _resourceManager.GetGameTexture("barrack")));
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
Vector2 Game::CalculateTotalFood() {
    Vector2 foodCounter = {0.0f, 0.0f};
    if (_farms.empty()) return foodCounter;
    for (auto& farm : _farms) {
        foodCounter.x += farm.GetCurrentResourceCount();
        foodCounter.y += farm.GetCapacity();
    }
    return foodCounter;
}

void Game::StartRaidEvent() {
    _isRaidActive = true;
    _raiders.push_back(Raider(Vector2{70.0f, 100.0f}, _resourceManager.GetGameTexture("raider")));
    for (auto& barrack: _barracks) {
        barrack.MobilizeKnights();
    }
}

void Game::HandleRaid() {
    if (IsKeyPressed(KEY_I)) {
        _raiders.push_back(Raider(Vector2{70.0f, 100.0f}, _resourceManager.GetGameTexture("raider")));
    }
    // менять указатель у рыцарей если они указывают на мертвого рейдера
//    for (auto& raider: _raiders) {
//        if (!raider.IsAlive()) {
//            std::cout << &raider - &_raiders[0] << " is dead\n";
//        }
//    }
    _raiders.erase(std::remove_if(_raiders.begin(), _raiders.end(), [](const Raider& raider) {
        return !raider.IsAlive();
    }), _raiders.end());
    _woodStorages.erase(std::remove_if(_woodStorages.begin(), _woodStorages.end(), [](const Storage& st) {
        return st.IsDestroyed();
    }), _woodStorages.end());
    _stoneStorages.erase(std::remove_if(_stoneStorages.begin(), _stoneStorages.end(), [](const Storage& st) {
        return st.IsDestroyed();
    }), _stoneStorages.end());
    _farms.erase(std::remove_if(_farms.begin(), _farms.end(), [](const Storage& st) {
        return st.IsDestroyed();
    }), _farms.end());
    // можно еще проверять что рыцарь возле рейдера но тогда он будет атаковать не блтжа йшего
    if (_raiders.empty()) {
        EndRaidEvent();
    }
}

void Game::EndRaidEvent() {
    _isRaidActive = false;
    std::cout << "EVENT ENDS!!!\n";
    for (auto& barrack: _barracks) {
        barrack.DemobilizeKnights();
    }
}

void Game::Reset() {
    _currentState = GameState::MainMenu;
    _raiders.clear();
    EndRaidEvent();
    _townhall = Townhall(Vector2{700.0f, 700.0f});
    _townhall.SetTexture(_resourceManager.GetGameTexture("townhall"));
}
