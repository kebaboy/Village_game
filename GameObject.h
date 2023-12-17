//
// Created by Григорий on 05.10.2023.
//

#ifndef VILLAGE_GAMEOBJECT_H
#define VILLAGE_GAMEOBJECT_H
#include "raylib.h"
#include <string>
#include "mathFunctions.h"
#include "Map.h"
#include "Resources.h"

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
    void SetPosition(Vector2 position);
    const Vector2& GetPosition() const;
    const Vector2& GetSize() const;
};

class Player: public GameObject {
private:
    const float _playerVelocity = 3.0f;
public:
    Player(const Vector2 pos, const Vector2 size);
    void Draw() const override;
    const float GetVelocity() const;
};

class Building: public GameObject {
protected:
    Requirements _requirements;
public:
    Building(const Vector2 pos, const Vector2 size, const Texture2D sprite);
    Building(const Vector2 pos, const Vector2 size);
    bool CanBuild(const Resources& availableResources);
    const Requirements& GetRequirements() const;
};


class WorkerHouse: public Building {
public:
    WorkerHouse(const Vector2 pos, const Texture2D sprite);
};

class LumberjackHouse: public WorkerHouse {
public:
    LumberjackHouse(const Vector2 pos, const Texture2D sprite);
};
class MinerHouse: public WorkerHouse {
public:
    MinerHouse(const Vector2 pos, const Texture2D sprite);
};
class FarmerHouse: public WorkerHouse {
public:
    FarmerHouse(const Vector2 pos, const Texture2D sprite);
};



class Storage: public Building {
protected:
    int _resourceCount = 0;
    int _maxCapacity = 20;
    float _durability = 5.0f;
    bool _destroyed = false;
public:
    Storage(const Vector2 pos, const Texture2D sprite);
    Storage(const Vector2 pos);

    virtual bool AddResource(int amount);
    int GetCurrentResourceCount() const;
    int GetCapacity() const;
    void TakeDamage(int damage);
    bool IsDestroyed() const;

    bool isFull() const;
};

class Townhall: public Storage {
public:
    Townhall(const Vector2 pos);
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
    int _workersAmount = 5;
    int _farmersCount = 0;
    int _maxFarmersCount = 2;
public:
    Farm(const Vector2 pos, const Texture2D sprite);
    int GetFarmersCount() const;
    int GetMaxFarmersCount() const;
    int GetWorkersAmount() const;
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
    TO_RAIDER,
    ATTACK,
};

class MovingGameObject: public GameObject {
protected:
    float _speed = 1.0f;
public:
    MovingGameObject(const Vector2 pos, const Vector2 size, const Texture2D sprite);

    virtual GameObject* FindClosestObject(const std::vector<GameObject*>& objects);
    void MoveForwardTarget(const Vector2 target);
    bool IsAtTarget(const Vector2 target);
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

    bool _hungry = false;

    Vector2 _homePosition;
public:
    Worker(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition);

    Vector2 FindClosestObject(const std::vector<Storage*>& storages);
    bool AddResourceToStorage(std::vector<Storage*>& storages);
    void SetHomePosition(Vector2 pos);

    void DecreaseEnergy(int delta = 1);

    void Draw() const override;

    void Hunger(bool value);
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
    Vector2 _targetFarmPosition = {-1, -1};
    Vector2 _collectingTarget = {-1, -1};
public:
    Farmer(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition);

    Farm* FindClosestFarm(std::vector<Farm>& farms);
    Farm* FindClosestTargetFarm(std::vector<Farm>& farms);

    void Update(std::vector<Farm>& farmStorages, Map& map);
};

class Warrior: public MovingGameObject {
protected:
    TaskMode _taskMode;
    float _hp = 5.0f;
    float _damage = 1.0f;
    float _attackRange = 10.0f;
    float _attackCooldown = 0.0f;
    float _attackCooldownDuration = 2.0f;
    float _damageFlashTime = 0.0f;
    float _maxDamageFlashTime = 0.5f;
    bool _alive = true;
public:
    Warrior(const Vector2 pos, const Texture2D sprite);
    bool IsAlive() const;
    virtual void TakeDamage(int damage);
    virtual void Attack(Warrior* target);
    void SetTaskMode (TaskMode task);
    TaskMode& GetTaskMode();
    void Draw() const override;
};

class Raider: public Warrior {
private:
public:
    Raider(const Vector2 pos, const Texture2D sprite);
    void Attack(Storage* target);
    void Update(std::vector<GameObject*> &targets, Map& map);

};

class Knight: public Warrior {
private:
    Vector2 _patrolPoint;
    float _waitTime = 3.0;
    Vector2 _barrackPosition;
public:
    Knight(const Vector2 pos, const Texture2D sprite, const Vector2 barrackPosition);
    Vector2 GetRandomPatrolPoint();
    Raider* FindClosestRaider(std::vector<Raider>& raiders);
    void Update(std::vector<Raider>& raiders);
};

class Barrack: public Storage {
private:
    std::vector<Knight> _knights;
    int _knightCost = 4;
public:
    Barrack(const Vector2 pos, const Texture2D sprite);
    void Update(ResourceManager& resourceManager, Camera2D& camera, std::vector<Raider>& raiders, bool isRaidActive, std::vector<Farm>& farms, const Resources& res);
    void MobilizeKnights();
    void DemobilizeKnights();
    bool AddResource (int count, std::vector<Farm>& farms, const Resources& res);
    void Draw() const override;
};

#endif //VILLAGE_GAMEOBJECT_H
