//
// Created by Григорий on 05.10.2023.
//

#include "Game.h"
#include "iostream"

Game::Game() : _screenWidth(1000), _screenHeight(650),
                _currentState(GameState::InGame),
                _menu(Menu(_screenWidth, _screenHeight)),
                _map(40, 40, 100),
                _player(Vector2{ (float)(20 * 100) + 53, (float)(20 * 100) + 110}, Vector2{float(50), float(50)}),
                _townhall(Vector2{(float)(20 * 100), (float)(20 * 100)}),
                _availableResources(Vector2{0, 0}, Vector2{0, 0}, Vector2{0, 0})
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
    _resourceManager.LoadGameTexture("church", "church.png");
    _resourceManager.LoadGameTexture("sleep", "sleep.png");
    _resourceManager.LoadGameTexture("start", "start_btn.png");
    _resourceManager.LoadGameTexture("game-over", "game-over.png");
    _resourceManager.LoadGameTexture("wheat", "wheat.png");
    _resourceManager.LoadGameTexture("stone-r", "stone-r.png");
    _resourceManager.LoadGameTexture("wood", "wood.png");
    _menu.Initialize(_resourceManager);
    _map.Generate();
    _player.SetTexture(_resourceManager.GetGameTexture("player"));
    _townhall.SetTexture(_resourceManager.GetGameTexture("church"));
    _woodStorages.push_back(WoodStorage(Vector2{_townhall.GetPosition().x - 170.0f, _townhall.GetPosition().y - 170.0f}, _resourceManager.GetGameTexture("wood_storage")));
    _woodStorages.back().AddResource(_woodStorages.back().GetCapacity());
    _stoneStorages.push_back(StoneStorage(Vector2{_townhall.GetPosition().x - 170.0f, _townhall.GetPosition().y}, _resourceManager.GetGameTexture("stone_storage")));
    _stoneStorages.back().AddResource(_stoneStorages.back().GetCapacity());
    _farms.push_back(Farm(Vector2{_townhall.GetPosition().x, _townhall.GetPosition().y - 170.0f}, _resourceManager.GetGameTexture("farm")));
    _farms.back().AddResource(_farms.back().GetCapacity());
    _camera = {0};
    _camera.target = _player.GetPosition();
    _camera.offset = Vector2{(float)_screenWidth/2, (float)_screenHeight/2};
    _camera.rotation = 0.0f;
    _camera.zoom = 1.0f;
    _ui.Initialize(Vector2 {(float)_screenWidth, (float)_screenHeight}, _resourceManager, this);
    SetTargetFPS(60);

    std::vector<ResourceInfo> lumberjackHouse = {
            {"wood", _resourceManager.GetGameTexture("wood"), 5},
            {"stone", _resourceManager.GetGameTexture("stone-r"), 3},
            {"wheat", _resourceManager.GetGameTexture("wheat"), 3}
    };
    std::vector<ResourceInfo> minerHouse = {
            {"wood", _resourceManager.GetGameTexture("wood"), 3},
            {"stone", _resourceManager.GetGameTexture("stone-r"), 5},
            {"wheat", _resourceManager.GetGameTexture("wheat"), 3}
    };
    std::vector<ResourceInfo> farmerHouse = {
            {"wood", _resourceManager.GetGameTexture("wood"), 5},
            {"stone", _resourceManager.GetGameTexture("stone-r"), 5},
            {"wheat", _resourceManager.GetGameTexture("wheat"), 3}
    };
    std::vector<ResourceInfo> woodStorage = {
            {"wood", _resourceManager.GetGameTexture("wood"), 15},
            {"stone", _resourceManager.GetGameTexture("stone-r"), 10},
            {"wheat", _resourceManager.GetGameTexture("wheat"), 0}
    };
    std::vector<ResourceInfo> stoneStorage = {
            {"wood", _resourceManager.GetGameTexture("wood"), 10},
            {"stone", _resourceManager.GetGameTexture("stone-r"), 15},
            {"wheat", _resourceManager.GetGameTexture("wheat"), 0}
    };
    std::vector<ResourceInfo> farm = {
            {"wood", _resourceManager.GetGameTexture("wood"), 7},
            {"stone", _resourceManager.GetGameTexture("stone-r"), 7},
            {"wheat", _resourceManager.GetGameTexture("wheat"), 15}
    };
    std::vector<ResourceInfo> barrack = {
            {"wood", _resourceManager.GetGameTexture("wood"), 12},
            {"stone", _resourceManager.GetGameTexture("stone-r"), 12},
            {"wheat", _resourceManager.GetGameTexture("wheat"), 5}
    };
//    std::vector<ResourceInfo> minerHouse = {
//            {"wood", _resourceManager.GetGameTexture("wood"), 50},
//            {"stone", _resourceManager.GetGameTexture("wood"), 30},
//            {"stone", _resourceManager.GetGameTexture("wood"), 30}
//    };

// Fill in the buildingResourceInfo map
    _buildingResourceInfo[BuildingType::LumberjackHouse] = lumberjackHouse;
    _buildingResourceInfo[BuildingType::MinerHouse] = minerHouse;
    _buildingResourceInfo[BuildingType::FarmerHouse] = farmerHouse;
    _buildingResourceInfo[BuildingType::WoodStorage] = woodStorage;
    _buildingResourceInfo[BuildingType::StoneStorage] = stoneStorage;
    _buildingResourceInfo[BuildingType::Farm] = farm;
    _buildingResourceInfo[BuildingType::Barrack] = barrack;
// ... add entries for other building types
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
    if (_elapsedRaidTime < _raidEventInterval) {
        _elapsedRaidTime += GetFrameTime();
    } else if (!_isRaidActive) {
        int knightsCount = 10;
        if (knightsCount >= 3) {
            StartRaidEvent(knightsCount - 2);
        }
    }

    if (_isRaidActive) {
        HandleRaid();
    }

    std::vector<Storage*> woodStorages;
    for (auto& ws : _woodStorages) {
        woodStorages.push_back(&ws);
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

    int extra = CalculateTotalWorkersAmount() - _lumberjacks.size() - _miners.size() - _farmers.size();
    std::vector<Worker*> allWorkers;
    for (auto& w: _lumberjacks) {
        allWorkers.push_back(&w);
    }
    for (auto& w: _miners) {
        allWorkers.push_back(&w);
    }
    for (auto& w: _farmers) {
        allWorkers.push_back(&w);
    }
    if (extra < 0) {
//        std::vector<int> ind;
//        extra *= -1;
//        while (ind.size() != extra) {
//            int index = GetRandomValue(0, allWorkers.size() - 1);
//            if ((std::find(ind.begin(), ind.end(), index)) == ind.end()) {
//                ind.push_back(index);
//            }
//        }
        extra = (extra > 0) ? extra : -extra;
        for (size_t i = 0; i < extra; i++) {
            allWorkers[allWorkers.size() - 1 - i]->Hunger(true);
        }
//        for (int index: ind) {
//            allWorkers[index]->Hunger(true);
//        }

    } else {
        for (auto worker: allWorkers) {
            worker->Hunger(false);
        }
    }
//    allWorkers.insert(allWorkers.end(), _lumberjacks.begin(), _lumberjacks.end());
//    allWorkers.insert(allWorkers.end(), _miners.begin(), _miners.end());
//    allWorkers.insert(allWorkers.end(), _farmers.begin(), _farmers.end());
    for (auto& lumberjack : _lumberjacks) {
        lumberjack.Update(woodStorages, _map);
    }
    for (auto& miner : _miners) {
        miner.Update(stoneStorages, _map);
    }
    for (auto& farmer: _farmers) {
        farmer.Update(_farms, _map);
    }

    for (auto& barrack: _barracks) {
        barrack.Update(_resourceManager, _camera, _raiders, _isRaidActive, _farms, _availableResources);
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

    CalculateAvailableResources();
    _townhall.Update();

    if (_townhall.IsDestroyed()) {
        _defeatMessageTimer += GetFrameTime();
        if (_defeatMessageTimer >= _defeatMessageDuration) {
            Reset();
        }
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
    _ui.Draw(_buildingResourceInfo);
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Wood: %d / %d\nStone: %d / %d\nFood: %d / %d", (int)_availableResources._wood.x, (int)_availableResources._wood.y, (int)_availableResources._stone.x, (int)_availableResources._stone.y, (int)_availableResources._food.x, (int)_availableResources._food.y);
    if (_isRaidActive) {
        char buffer2[20];
        snprintf(buffer2, sizeof(buffer2), "Raiders: %d", (int)_raiders.size());
        DrawText(buffer2, _screenWidth / 2 - 70, 20, 40, RED);
    }
    DrawText(buffer, 20, 20, 20, WHITE);

    DrawRectangle(20, 110, 150, 20, DARKGRAY);
    int workers = CalculateTotalWorkersAmount();
    int workersCount = _lumberjacks.size()+_miners.size()+_farmers.size();
    if (workers && workersCount <= workers && !_flashRed) {
        DrawRectangle(20, 110, 150, 20, YELLOW);
        DrawRectangle(20, 110, static_cast<int>((150.0f / workers) * (workersCount)), 20, DARKGRAY);
    } else if (_flashTimer < _flashDuration) {
        DrawRectangle(20, 110, 150, 20, RED);
        _flashTimer += GetFrameTime();
    } else _flashRed = false;

    DrawFPS(20, 140);


    if (_townhall.IsDestroyed()) DrawTexturePro(_resourceManager.GetGameTexture("game-over"), Rectangle{0.0f, 0.0f, (float)_resourceManager.GetGameTexture("game-over").width, (float)_resourceManager.GetGameTexture("game-over").height}, Rectangle{(float)_screenWidth / 2 - 800.0f / 2, (float)_screenHeight / 2 - 150.0f / 2, 800.0f, 150.0f}, Vector2{0, 0}, 0.0f, WHITE);

    EndDrawing();
}

void Game::PlaceBuilding(Vector2 position) {
    if (!_buildingPlacingMode) return;
    Vector2 worldPos = GetScreenToWorld2D(position, _camera);
    switch(_previewBuildingType) {
        case BuildingType::WoodStorage: {
            WoodStorage obj = WoodStorage(worldPos, _resourceManager.GetGameTexture("wood_storage"));
            if (obj.CanBuild(_availableResources)) {
                DecreaseAvailableResources(obj.GetRequirements());
                _woodStorages.push_back(obj);
            }
            break;
        }
        case BuildingType::StoneStorage: {
            StoneStorage obj = StoneStorage(worldPos, _resourceManager.GetGameTexture("stone_storage"));
            if (obj.CanBuild(_availableResources)) {
                DecreaseAvailableResources(obj.GetRequirements());
                _stoneStorages.push_back(obj);
            }
            break;
        }
        case BuildingType::Farm: {
            Farm obj = Farm(worldPos, _resourceManager.GetGameTexture("farm"));
            if (obj.CanBuild(_availableResources)) {
                DecreaseAvailableResources(obj.GetRequirements());
                _farms.push_back(obj);
            }
            break;
        }
        case BuildingType::LumberjackHouse:
            if (_lumberjacks.size() + _miners.size() + _farmers.size() < CalculateTotalWorkersAmount()) {
                LumberjackHouse obj = LumberjackHouse(worldPos, _resourceManager.GetGameTexture("lumberjack_house"));
                if (obj.CanBuild(_availableResources)) {
                    DecreaseAvailableResources(obj.GetRequirements());
                    _lumberjackHouses.push_back(obj);
                    _lumberjacks.push_back(Lumberjack(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("lumberjack"), worldPos));
                    _lumberjacks.back().SetHomePosition(worldPos);
                }
            } else {
                _flashRed = true;
                _flashTimer = 0.0f;
            }
            break;
        case BuildingType::MinerHouse:
            if (_lumberjacks.size() + _miners.size() + _farmers.size() < CalculateTotalWorkersAmount()) {
                MinerHouse obj = MinerHouse(worldPos, _resourceManager.GetGameTexture("miner_house"));
                if (obj.CanBuild(_availableResources)) {
                    DecreaseAvailableResources(obj.GetRequirements());
                    _minerHouses.push_back(obj);
                    _miners.push_back(Miner(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("miner"), worldPos));
                    _miners.back().SetHomePosition(worldPos);
                }
            } else {
                _flashRed = true;
                _flashTimer = 0.0f;
            }
            break;
        case BuildingType::FarmerHouse:
            if (_lumberjacks.size() + _miners.size() + _farmers.size() < CalculateTotalWorkersAmount()) {
                FarmerHouse obj = FarmerHouse(worldPos, _resourceManager.GetGameTexture("farmer_house"));
                if (obj.CanBuild(_availableResources)) {
                    DecreaseAvailableResources(obj.GetRequirements());
                    _farmerHouses.push_back(obj);
                    _farmers.push_back(Farmer(Vector2 {worldPos.x + 50, worldPos.y + 50}, _resourceManager.GetGameTexture("farmer"), worldPos));
                    _farmers.back().SetHomePosition(worldPos);
                }
            } else {
                _flashRed = true;
                _flashTimer = 0.0f;
            }
            break;
        case BuildingType::Barrack:
            Barrack obj = Barrack(worldPos, _resourceManager.GetGameTexture("barrack"));
            if (obj.CanBuild(_availableResources)) {
                DecreaseAvailableResources(obj.GetRequirements());
                _barracks.push_back(obj);
            }
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

void Game::CalculateAvailableResources() {
    _availableResources._wood = CalculateTotalWood();
    _availableResources._stone = CalculateTotalStone();
    _availableResources._food = CalculateTotalFood();
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

void Game::StartRaidEvent(int knightsCount) {
    _isRaidActive = true;
    Vector2 mapSize = _map.GetMapSize();
    for (int i = 0; i < knightsCount; i++) {
        int randomSide = GetRandomValue(0, 3);
        float randomX, randomY;
        switch (randomSide) {
            case 0:  // Left side
                randomX = static_cast<float>(GetRandomValue(-80, -50));
                randomY = static_cast<float>(GetRandomValue(0, mapSize.y));
                break;
            case 1:  // Right side
                randomX = static_cast<float>(GetRandomValue(mapSize.x, mapSize.x + 30));
                randomY = static_cast<float>(GetRandomValue(0, mapSize.y));
                break;
            case 2:  // Top side
                randomX = static_cast<float>(GetRandomValue(0, mapSize.x));
                randomY = static_cast<float>(GetRandomValue(-80, -50));
                break;
            case 3:  // Bottom side
                randomX = static_cast<float>(GetRandomValue(0, mapSize.x));
                randomY = static_cast<float>(GetRandomValue(mapSize.y, mapSize.y + 30));
                break;
        }
        std::cout << randomX << randomY << std::endl;
        _raiders.push_back(Raider(Vector2{randomX, randomY}, _resourceManager.GetGameTexture("raider")));
    }
    for (auto& barrack: _barracks) {
        barrack.MobilizeKnights();
    }
}

void Game::HandleRaid() {
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
    _elapsedRaidTime = 0.0f;
}

void Game::Reset() {
    _currentState = GameState::MainMenu;
    _isRaidActive = false;
    _defeatMessageTimer = 0.0f;
    _raiders.clear();
    _barracks.clear();
    _lumberjacks.clear();
    _miners.clear();
    _farmers.clear();
    _lumberjackHouses.clear();
    _minerHouses.clear();
    _farmerHouses.clear();
    _woodStorages.clear();
    _stoneStorages.clear();
    _farms.clear();
    _buildingPlacingMode = false;
    _ui.Reset();

    _elapsedRaidTime = 0.0f;
    _townhall = Townhall(Vector2{(float)(20 * 100), (float)(20 * 100)});
    _townhall.SetTexture(_resourceManager.GetGameTexture("church"));
    _player.SetPosition(Vector2{(float)(20 * 100) + _townhall.GetSize().x / 2 - 23, (float)(20 * 100) + 110});
    _player.SetTexture(_resourceManager.GetGameTexture("player"));

    _woodStorages.push_back(WoodStorage(Vector2{_townhall.GetPosition().x - 170.0f, _townhall.GetPosition().y - 170.0f}, _resourceManager.GetGameTexture("wood_storage")));
    _woodStorages.back().AddResource(_woodStorages.back().GetCapacity());
    _stoneStorages.push_back(StoneStorage(Vector2{_townhall.GetPosition().x - 170.0f, _townhall.GetPosition().y}, _resourceManager.GetGameTexture("stone_storage")));
    _stoneStorages.back().AddResource(_stoneStorages.back().GetCapacity());
    _farms.push_back(Farm(Vector2{_townhall.GetPosition().x, _townhall.GetPosition().y - 170.0f}, _resourceManager.GetGameTexture("farm")));
    _farms.back().AddResource(_farms.back().GetCapacity());
}

int Game::CalculateTotalWorkersAmount() {
    int workers = std::accumulate(_farms.begin(), _farms.end(), 0,
                              [](int partialSum, const Farm& obj) {
                                  return partialSum + obj.GetWorkersAmount();
                              });
    return workers;
}

int Game::CalculateTotalKnights() {
    int n = 0;
    for (const auto& barrack: _barracks) {
        n += barrack.GetCurrentResourceCount();
    }
    return n;
}

void Game::DecreaseAvailableResources(Requirements req) {
    if (req._wood != 0) {
        while (req._wood > 0) {
            for (auto& st: _woodStorages) {
                int count = st.GetCurrentResourceCount();
                if (count != 0) {
                    if (count < req._wood) {
                        req._wood -= count;
                        st.AddResource(count * -1);
                    }
                    else {
                        st.AddResource(req._wood * -1);
                        req._wood = 0;
                    }
                }
            }
        }
    }
    if (req._stone != 0) {
        while (req._stone > 0) {
            for (auto& st: _stoneStorages) {
                int count = st.GetCurrentResourceCount();
                if (count != 0) {
                    if (count < req._stone) {
                        req._stone -= count;
                        st.AddResource(count * -1);
                    }
                    else {
                        st.AddResource(req._stone * -1);
                        req._stone = 0;
                    }
                }
            }
        }
    }
    if (req._food != 0) {
        while (req._food > 0) {
            for (auto& st: _farms) {
                int count = st.GetCurrentResourceCount();
                if (count != 0) {
                    if (count < req._food) {
                        req._food -= count;
                        st.AddResource(count * -1);
                    }
                    else {
                        st.AddResource(req._food * -1);
                        req._food = 0;
                    }
                }
            }
        }
    }
}