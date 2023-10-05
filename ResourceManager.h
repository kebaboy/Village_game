//
// Created by Григорий on 05.10.2023.
//

#ifndef VILLAGE_RESOURCEMANAGER_H
#define VILLAGE_RESOURCEMANAGER_H
#include <string>
#include <map>
#include <filesystem>
#include "raylib.h"

class ResourceManager {
private:
    std::string _texturesPath = "../resources/textures/";
    std::map<std::string, Texture2D> _textures;
public:
    ~ResourceManager();
    void LoadGameTexture(const std::string& id, const std::string& fileName);
    Texture2D& GetGameTexture(const std::string& id);
    void UnloadGameTexture(const std::string& id);
};


#endif //VILLAGE_RESOURCEMANAGER_H
