//
// Created by Григорий on 05.10.2023.
//

#ifndef VILLAGE_GAMEOBJECT_H
#define VILLAGE_GAMEOBJECT_H
#include "raylib.h"
#include <string>

class GameObject {
protected:
    Vector2 _position;
    Texture2D _sprite;
public:
    GameObject(const Vector2 pos);
    virtual void Update();
    virtual void Draw();
    void SetTexture(const Texture2D& texture);
    Vector2 GetPositon() const;
};

class Player: public GameObject {
private:
    const float _playerVelocity = 3.0f;

public:
    Player(const Vector2 pos);
    void Update() override;
    void Draw() override;
};

#endif //VILLAGE_GAMEOBJECT_H
