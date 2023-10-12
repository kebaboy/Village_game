//
// Created by Григорий on 11.10.2023.
//

#include "Map.h"

Map::Map(int w, int h, float tileSize): _width(w), _height(h), _tileSize(tileSize) {
    _tiles.resize(_height, std::vector<Tile>(_width, Tile({0, 0}, TileType::GRASS)));
}

void Map::Generate() {
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            int r = GetRandomValue(0, 10);
            if (r < 2) _tiles[y][x] = Tile({ x * _tileSize, y * _tileSize }, TileType::GRASS, TileType::TREE);
            else if (r < 4) _tiles[y][x] = Tile({ x * _tileSize, y * _tileSize}, TileType::GRASS, TileType::STONE);
            else _tiles[y][x] = Tile({ x * _tileSize, y * _tileSize}, TileType::GRASS);
        }
    }
}

void Map::Draw(ResourceManager& resourceManager) {
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            _tiles[y][x].Draw(resourceManager, _tileSize);
        }
    }
}

Tile::Tile(Vector2 pos, TileType base, TileType overlay): _position(pos), _baseType(base), _overlayType(overlay) {}

std::string Tile::baseTypeToString(TileType type) {
    switch(type) {
        case TileType::GRASS: return "grass";
        case TileType::TREE: return "tree";
        case TileType::STONE: return "stone";
        default: return "";
    }
}

void Tile::Draw(ResourceManager& resourceManager, float tileSize) {
    DrawTexturePro(resourceManager.GetGameTexture(baseTypeToString(_baseType)), Rectangle{0.0f, 0.0f, (float)resourceManager.GetGameTexture(baseTypeToString(_baseType)).width, (float)resourceManager.GetGameTexture(baseTypeToString(_baseType)).height}, Rectangle{_position.x, _position.y, tileSize, tileSize}, Vector2{0,0}, 0.0f, WHITE);
    if(_overlayType != TileType::EMPTY) {
        DrawTexturePro(resourceManager.GetGameTexture(baseTypeToString(_overlayType)), Rectangle{0.0f, 0.0f, (float)resourceManager.GetGameTexture(baseTypeToString(_overlayType)).width, (float)resourceManager.GetGameTexture(baseTypeToString(_overlayType)).height}, Rectangle{_position.x, _position.y, tileSize, tileSize}, Vector2{0,0}, 0.0f, WHITE);
    }
}