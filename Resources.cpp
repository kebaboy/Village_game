//
// Created by Григорий on 07.12.2023.
//

#include "Resources.h"

Resources::Resources(): _wood(Vector2{0, 0}), _stone(Vector2{0, 0}), _food(Vector2{0, 0}) {}
Resources::Resources(Vector2 wood, Vector2 stone, Vector2 food): _wood(wood), _stone(stone), _food(food) {}

Requirements::Requirements(): _wood(0), _stone(0), _food(0) {}
Requirements::Requirements(int wood, int stone, int food): _wood(wood), _stone(stone), _food(food) {}