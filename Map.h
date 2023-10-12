//
// Created by Григорий on 11.10.2023.
//

#ifndef VILLAGE_MAP_H
#define VILLAGE_MAP_H
#include "raylib.h"
#include "vector"
#include "ResourceManager.h"

enum class TileType {
    EMPTY,
    TREE,
    STONE,
    GRASS,
    SAND
};

class Tile {
private:
    TileType _baseType;
    TileType _overlayType;
    Vector2 _position;
public:
    Tile(Vector2 pos, TileType base, TileType overlay = TileType::EMPTY);
    std::string baseTypeToString(TileType type);
    void Draw(ResourceManager& resourceManager, float tileSize);
};

class Map {
private:
    int _width;
    int _height;
    std::vector<std::vector<Tile>> _tiles;
    float _tileSize;
    ResourceManager _resourceManager;
public:
    Map(int w, int h, float tileSize);
    void Generate();
    void Draw(ResourceManager& resourceManager);
    Vector2 GetMapSize() {
        return Vector2{_width * _tileSize, _height * _tileSize};
    }
};


#endif //VILLAGE_MAP_H
