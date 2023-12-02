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

class Game {
private:
    int _screenWidth;
    int _screenHeight;
    ResourceManager _resourceManager;
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

    Vector2 _woodCounter;
    Vector2 _stoneCounter;
    Vector2 _foodCounter;
public:
    Game();
    void Initialize();
    void Update();
    void UpdateCamera();
    void Draw();
    void Run();
    void PlayerMove(Vector2 direction);

    void PlaceBuilding(Vector2 position);
    void ToggleBuildingPlacementMode(BuildingType type);
    Vector2 GetMapSize() const;
    Vector2 CalculateTotalWood();
    Vector2 CalculateTotalStone();
    Vector2 CalculateTotalFood();

    void StartRaidEvent();
    void HandleRaid();
    void EndRaidEvent();
};


#endif //VILLAGE_GAME_H
