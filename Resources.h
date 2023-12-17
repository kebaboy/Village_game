//
// Created by Григорий on 07.12.2023.
//

#ifndef VILLAGE_RESOURCES_H
#define VILLAGE_RESOURCES_H
#include "raylib.h"

class Resources {
public:
    Vector2 _wood;
    Vector2 _stone;
    Vector2 _food;
    Resources();
    Resources(Vector2 wood, Vector2 stone, Vector2 food);
};

class Requirements {
public:
    int _wood;
    int _stone;
    int _food;
    Requirements();
    Requirements(int wood, int stone, int food);
};


#endif //VILLAGE_RESOURCES_H
