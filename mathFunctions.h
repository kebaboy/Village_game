//
// Created by Григорий on 12.10.2023.
//

#ifndef VILLAGE_MATHFUNCTIONS_H
#define VILLAGE_MATHFUNCTIONS_H
#include "raylib.h"
#include "math.h"

inline float Vector2Distance(Vector2 v1, Vector2 v2) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    return sqrt(dx*dx + dy*dy);
}

inline bool IsEqual(Vector2 v1, Vector2 v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

#endif //VILLAGE_MATHFUNCTIONS_H
