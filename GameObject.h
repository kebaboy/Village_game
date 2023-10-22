//
// Created by Григорий on 05.10.2023.
//

#ifndef VILLAGE_GAMEOBJECT_H
#define VILLAGE_GAMEOBJECT_H
#include "raylib.h"
#include <string>
#include "mathFunctions.h"

class GameObject {
protected:
    Vector2 _position;
    Vector2 _size;
    Texture2D _sprite;
public:
    GameObject(const Vector2 pos, const Vector2 size);
    GameObject(const Vector2 pos, const Vector2 size, const Texture2D sprite);
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



class WorkerHouse: public GameObject {
protected:
    int _durability = 100;
    const int _maxDurability = 100;
public:

    WorkerHouse(const Vector2 pos, const Texture2D sprite);

    virtual int GetCostFood() const = 0;
    virtual int GetCostWood() const = 0;
    virtual int GetCostStone() const = 0;
//    void DrawOpacity() const;
};

class LumberjackHouse: public WorkerHouse {
public:
    LumberjackHouse(const Vector2 pos, const Texture2D sprite);

    int GetCostFood() const override;
    int GetCostWood() const override;
    int GetCostStone() const override;
};




class Storage: public GameObject {
protected:
    int _resourceCount = 0;
    int _maxCapacity = 100;
public:
    Storage(const Vector2 pos, const Texture2D sprite);

    bool AddResource(int amount);
    int GetCurrentResourceCount() const;
};

class WoodStorage: public Storage {
public:
    WoodStorage(const Vector2 pos, const Texture2D sprite);
};


//class Worker: public GameObject {
//private:
//    float _speed = 1.0f;
//    float _fatigue = ;
//    float _restTime;
//public:
//    Worker(const Vector2 pos, const Vector2 size, const Texture2D sprite);
//};


enum class LumberjackTaskMode {
    TO_TREE,
    TO_HOME,
    CHOPPING,
    RESTING,
    DELIVERING,
};
class Lumberjack : public GameObject {
private:
    float _speed = 1.0f;
    float _fatigue;
    float _restTime;

    Vector2 _housePosition;
    Vector2 _targetTreePosition;
    Vector2 _currentTree;
    LumberjackTaskMode _taskMode;
    float _choppingTime = 0.0f;
    float _timeToChop = 3.0f;
    float _choppingAmount = 1.0f;
public:
    Lumberjack(const Vector2 pos, const Vector2 size, const Texture2D sprite);

    void SetTargetTreePosition(Vector2 pos);
//    void MoveToward

    void Draw() const override;
    void Update(int& woodCount);
};

class Tree: public GameObject {
private:
    int _resources;
public:
    Tree(const Vector2 pos, const Vector2 size, const Texture2D sprite);
//    void DrawOpacity() const;
};


#endif //VILLAGE_GAMEOBJECT_H
