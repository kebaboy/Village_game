//
// Created by Григорий on 05.10.2023.
//

#ifndef VILLAGE_GAME_H
#define VILLAGE_GAME_H
#include "ResourceManager.h"
#include "GameObject.h"

class Game {
private:
    int _screenWidth;
    int _screenHeight;
    ResourceManager _resourceManager;
    int _map[10][10] = {
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    };
    int _tileSize;
    Player _player;
public:
    Game();
    void Initialize();
    void Update();
    void Draw();
    void Run();
};


#endif //VILLAGE_GAME_H
