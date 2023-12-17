//
// Created by Григорий on 05.10.2023.
//

#ifndef VILLAGE_GAME_H
#define VILLAGE_GAME_H
#include "ResourceManager.h"
#include "GameObject.h"
#include "Map.h"
#include "memory"
#include "BuildingType.h"
#include "UI.h"
#include "Menu.h"
#include "Resources.h"
#include "numeric"

class Game {
private:
    int _screenWidth;
    int _screenHeight;
    ResourceManager _resourceManager;
    enum GameState {
        MainMenu,
        InGame,
    };
    GameState _currentState;
    Menu _menu;
    UI _ui;
    Camera2D _camera;

    Map _map;
    Player _player;

    Townhall _townhall;

    std::vector<LumberjackHouse> _lumberjackHouses;
    std::vector<MinerHouse> _minerHouses;
    std::vector<FarmerHouse> _farmerHouses;

    std::vector<WoodStorage> _woodStorages;
    std::vector<StoneStorage> _stoneStorages;
    std::vector<Farm> _farms;
    std::vector<Barrack> _barracks;

    std::vector<Lumberjack> _lumberjacks;
    std::vector<Miner> _miners;
    std::vector<Farmer> _farmers;
    std::vector<Raider> _raiders;

    bool _buildingPlacingMode = false;
    BuildingType _previewBuildingType;

    bool _isRaidActive = false;

    Resources _availableResources;
//    Vector2 _woodCounter;
//    Vector2 _stoneCounter;
//    Vector2 _foodCounter;

    bool _flashRed = false;
    float _flashDuration = 0.5f;
    float _flashTimer = 0.0f;

    float _elapsedRaidTime = 0.0f;
    const float _raidEventInterval = 180.0f;
public:
    Game();
    void Initialize();
    void Update();
    void HandleGame();
    void HandleMenu();
    void HandleStates();
    void UpdateCamera();
    void Render();
    void Draw();
    void Run();
    void PlayerMove(Vector2 direction);

    void PlaceBuilding(Vector2 position);
    void ToggleBuildingPlacementMode(BuildingType type);
    Vector2 GetMapSize() const;

    void CalculateAvailableResources();
    Vector2 CalculateTotalWood();
    Vector2 CalculateTotalStone();
    Vector2 CalculateTotalFood();
    int CalculateTotalWorkersAmount();
    int CalculateTotalKnights();
    void DecreaseAvailableResources(Requirements req);

    void StartRaidEvent(int knightsCount);
    void HandleRaid();
    void EndRaidEvent();

    void Reset();
};


#endif //VILLAGE_GAME_H
