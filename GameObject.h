//
// Created by Григорий on 05.10.2023.
//

#ifndef VILLAGE_GAMEOBJECT_H
#define VILLAGE_GAMEOBJECT_H
#include "raylib.h"
#include <string>
#include "mathFunctions.h"
#include "Map.h"

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
    const Vector2& GetPosition() const;
    const Vector2& GetSize() const;
    void SetPosition(Vector2 position);
};

class MovingGameObject: public GameObject {
protected:
    float _speed = 1.0f;
public:
    MovingGameObject(const Vector2 pos, const Vector2 size, const Texture2D sprite);
    void MoveForwardTarget(const Vector2 target);
    bool IsAtTarget(const Vector2 target);
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
class MinerHouse: public WorkerHouse {
public:
    MinerHouse(const Vector2 pos, const Texture2D sprite);

    int GetCostFood() const override;
    int GetCostWood() const override;
    int GetCostStone() const override;
};
class FarmerHouse: public WorkerHouse {
public:
    FarmerHouse(const Vector2 pos, const Texture2D sprite);

    int GetCostFood() const override;
    int GetCostWood() const override;
    int GetCostStone() const override;
};




class Storage: public GameObject {
protected:
    int _resourceCount = 0;
    int _maxCapacity = 3;
public:
    Storage(const Vector2 pos, const Texture2D sprite);

    bool AddResource(int amount);
    int GetCurrentResourceCount() const;
    int GetCapacity() const;

    bool isFull() const;
};

class WoodStorage: public Storage {
public:
    WoodStorage(const Vector2 pos, const Texture2D sprite);
};
class StoneStorage: public Storage {
public:
    StoneStorage(const Vector2 pos, const Texture2D sprite);
};
class Farm: public Storage {
private:
    int _farmersCount = 0; // можно убрать
    int _maxFarmersCount = 2;
public:
    Farm(const Vector2 pos, const Texture2D sprite);
    int GetFarmersCount() const; // можно убрать
    int GetMaxFarmersCount() const;
    bool AddFarmer();
    bool RemoveFarmer();
};

enum class TaskMode {
    TO_TREE,
    TO_STONE,
    TO_FARM,
    TO_HOME,
    COLLECTING,
    RESTING,
    DELIVERING,
    IDLE,
    PATROLLING,
};

class Worker: public MovingGameObject {
protected:
    TaskMode _taskMode;

    float _maxEnergy = 5;
    float  _energy = _maxEnergy;
    float _resourceAmount = 1.0f;

    float _restingTime = 0.0f;
    float _timeToRest = 5.0f;
    float _collectingTime = 0.0f;
    float _timeToCollect = 3.0f;

    Vector2 _homePosition;
public:
    Worker(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition);

    virtual Vector2 FindClosestStorage(const std::vector<Storage*>& storages);
    bool AddResourceToStorage(std::vector<Storage*>& storages);
    void SetHomePosition(Vector2 pos);

    void DecreaseEnergy(int delta = 1);
};

class Lumberjack : public Worker {
private:
public:
    Lumberjack(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition);

    void Update(std::vector<Storage*>& woodStorages, Map& map);
};
class Miner : public Worker {
private:
public:
    Miner(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition);

    void Update(std::vector<Storage*>& stoneStorages, Map& map);
};

class Farmer: public Worker {
private:
    int _currentFarmInd = -1;
    Vector2 _collectingTarget = {-1, -1};
public:
    Farmer(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition);

    void FindClosestFarm(std::vector<Farm>& farms);

    void Update(std::vector<Farm>& farmStorages, Map& map);
};

class Warrior: public MovingGameObject {
protected:
    TaskMode _taskMode;
    float _hp = 5.0f;
    float _damage = 1.0f;
    bool _alive = true;
public:
    Warrior(const Vector2 pos, const Texture2D sprite);
    bool IsAlive() const;
    virtual void TakeDamage(int damage);
    virtual void Attack(Warrior& target);
};

class Knight: public Warrior {
private:
    Vector2 _patrolPoint;
    float _waitTime = 3.0;
    Vector2 _barrackPosition;
public:
    Knight(const Vector2 pos, const Texture2D sprite, const Vector2 barrackPosition);
    Vector2 GetRandomPatrolPoint();

    void Update() override;
};

class Barrack: public Storage {
private:
    std::vector<Knight> _knights;
public:
    Barrack(const Vector2 pos, const Texture2D sprite);
    void Update(ResourceManager& resourceManager, Camera2D& camera);
    void Draw() const override;
};

class Tree: public GameObject {
private:
    int _resources;
public:
    Tree(const Vector2 pos, const Vector2 size, const Texture2D sprite);
//    void DrawOpacity() const;
};

#endif //VILLAGE_GAMEOBJECT_H
