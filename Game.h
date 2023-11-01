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
    std::vector<Tree> _trees;

    std::vector<LumberjackHouse> _lumberjackHouses;
    std::vector<MinerHouse> _minerHouses;

    std::vector<WoodStorage> _woodStorages;
    std::vector<StoneStorage> _stoneStorages;

    std::vector<Lumberjack> _lumberjasks;
    std::vector<Miner> _miners;

    bool _buildingPlacingMode = false;
    BuildingType _previewBuildingType;

    Vector2 _woodCounter;
    Vector2 _stoneCounter;
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
};


#endif //VILLAGE_GAME_H
