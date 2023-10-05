//
// Created by Григорий on 05.10.2023.
//

#include "ResourceManager.h"

ResourceManager::~ResourceManager() {
    for (auto& pair : _textures) {
        UnloadTexture(pair.second);
    }
}

void ResourceManager::LoadGameTexture(const std::string &id, const std::string &fileName) {
    std::string fullPath = _texturesPath + fileName;
    Texture2D texture = LoadTexture(fullPath.c_str());
    if (texture.id != 0) {
        _textures[id] = texture;
    } else throw std::invalid_argument(fileName + " image can not be loaded");
}

Texture2D& ResourceManager::GetGameTexture(const std::string &id) {
    return _textures[id];
}

void ResourceManager::UnloadGameTexture(const std::string &id) {
    UnloadTexture(_textures[id]);
    _textures.erase(id);
}


