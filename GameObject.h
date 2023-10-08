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
    Vector2 _size;
public:
    GameObject(const Vector2 pos, const Vector2 size);
    virtual void Update();
    virtual void Draw() const;
    void SetTexture(const Texture2D& texture);
    const Vector2& GetPositon() const;
    const Vector2& GetSize() const;
    void SetPosition(Vector2 position);
};

class Player: public GameObject {
private:
    const float _playerVelocity = 3.0f;
public:
    Player(const Vector2 pos, const Vector2 size);
    void Draw() const override;
    const float GetVelocity() const;
};

class House: public GameObject {
public:
    House(const Vector2 pos, const Vector2 size, const Texture2D sprite);
//    void DrawOpacity() const;
};
#endif //VILLAGE_GAMEOBJECT_H
