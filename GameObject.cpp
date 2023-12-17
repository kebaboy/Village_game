//
// Created by Григорий on 05.10.2023.
//

#include "GameObject.h"
#include "ResourceManager.h"
#include "iostream"

GameObject::GameObject(const Vector2 pos, const Vector2 size): _position(pos), _size(size) {}

GameObject::GameObject(const Vector2 pos, const Vector2 size, const Texture2D sprite): _position(pos), _size(size),
                                                                                       _sprite(sprite) {}

void GameObject::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

void GameObject::Update() {};

const Vector2& GameObject::GetPosition() const {
    return _position;
}

const Vector2& GameObject::GetSize() const {
    return _size;
}

void GameObject::SetTexture(const Texture2D& texture) {
    _sprite = texture;
}

void GameObject::SetPosition(Vector2 position) {
    _position = position;
}

MovingGameObject::MovingGameObject(const Vector2 pos, const Vector2 size, const Texture2D sprite): GameObject(pos, size, sprite) {}

GameObject* MovingGameObject::FindClosestObject(const std::vector<GameObject *> &objects) {
    GameObject* closestObject = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (const auto& object: objects) {
        float distance = Vector2Distance(_position, object->GetPosition());
        if (distance < closestDistance) {
            closestDistance = distance;
            closestObject = object;
        }
    }
    return closestObject;
}

void MovingGameObject::MoveForwardTarget(const Vector2 target) {
    Vector2 direction = {
            target.x - _position.x,
            target.y - _position.y
    };

    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= magnitude;
    direction.y /= magnitude;

    _position.x += direction.x * _speed;
    _position.y += direction.y * _speed;
}

bool MovingGameObject::IsAtTarget(const Vector2 target) {
    if (Vector2Distance(_position, target) < _speed) {
        return true;
    }
    return false;
}

Player::Player(const Vector2 pos, const Vector2 size) : GameObject(pos, size) {}

void Player::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width/4, (float)_sprite.height/4 - 7}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

const float Player::GetVelocity() const {
    return _playerVelocity;
}

Building::Building(const Vector2 pos, const Vector2 size, const Texture2D sprite): GameObject(pos, size, sprite) {}

Building::Building(const Vector2 pos, const Vector2 size): GameObject(pos, size) {}

bool Building::CanBuild(const Resources &availableResources) {
    return availableResources._wood.x >= _requirements._wood &&
           availableResources._stone.x >= _requirements._stone &&
           availableResources._food.x >= _requirements._food;
}

const Requirements &Building::GetRequirements() const {
    return _requirements;
}


WorkerHouse::WorkerHouse(const Vector2 pos, const Texture2D sprite): Building(pos, Vector2 {100.0f, 100.0f}, sprite) {}

//void House::DrawOpacity() const {
//    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{GetMousePosition().x, GetMousePosition().y, _size.x, _size.y}, Vector2{0,0}, 0.0f, Fade(WHITE, 0.5f));
//}

LumberjackHouse::LumberjackHouse(const Vector2 pos, const Texture2D sprite): WorkerHouse(pos, sprite) {_requirements = Requirements(5, 3, 3);}

MinerHouse::MinerHouse(const Vector2 pos, const Texture2D sprite): WorkerHouse(pos, sprite) {_requirements = Requirements(3, 5, 3);}

FarmerHouse::FarmerHouse(const Vector2 pos, const Texture2D sprite): WorkerHouse(pos, sprite) {_requirements = Requirements(5, 5, 3);}


Storage::Storage(const Vector2 pos, const Texture2D sprite): Building(pos, Vector2 {150.0f, 150.0f}, sprite) {}
Storage::Storage(const Vector2 pos): Building(pos, Vector2 {150.0f, 150.0f}) {}

bool Storage::AddResource(int amount) {

    std::cout << _resourceCount << " + " << amount << " / " << _maxCapacity << "\n" << _position.x << " " << _position.y << "\n";
    if (_resourceCount + amount <= _maxCapacity) {
        _resourceCount += amount;
        return true;
    }
    return false;
}

int Storage::GetCurrentResourceCount() const {
    return _resourceCount;
}

int Storage::GetCapacity() const {
    return _maxCapacity;
}

bool Storage::isFull() const {
    return _resourceCount >= _maxCapacity;
}

bool Storage::IsDestroyed() const {
    return _destroyed;
}

void Storage::TakeDamage(int damage) {
    _durability -= damage;
    if (_durability <= 0) {
        _destroyed = true;
        std::cout << "DESTROYED!\n";
    }
    std::cout << "dur: " << _durability << "\n";
}

Townhall::Townhall(const Vector2 pos): Storage(pos) {};

WoodStorage::WoodStorage(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {_requirements = Requirements(15, 10, 0);}

StoneStorage::StoneStorage(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {_requirements = Requirements(10, 15, 0);}

Farm::Farm(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {_requirements = Requirements(7, 7, 15);}

int Farm::GetFarmersCount() const {
    return _farmersCount;
}

int Farm::GetMaxFarmersCount() const {
    return _maxFarmersCount;
}

int Farm::GetWorkersAmount() const {
    return _workersAmount;
}

bool Farm::AddFarmer() {
    _farmersCount++;
}

bool Farm::RemoveFarmer() {
    _farmersCount--;
}

Barrack::Barrack(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {
    _requirements = Requirements(12, 12, 5);
    _resourceCount = 0;
    _maxCapacity = 5;
}

bool Barrack::AddResource(int amount, std::vector<Farm>& farms, const Resources& res) {
    if (_resourceCount + amount <= _maxCapacity && res._food.x >= _knightCost) {
        int value = _knightCost;
        for (auto& farm: farms) {
            int count = farm.GetCurrentResourceCount();
            if (count != 0) {
                if (count >= value) {
                    farm.AddResource(value * -1);
                    value = 0;
                    break;
                }
                else {
                    value -= count;
                    farm.AddResource(count * -1);
                }
            }
        }
        if (value == 0) {
            _resourceCount += amount;
            return true;
        }
    }
    return false;
}

void Barrack::Update(ResourceManager& resourceManager, Camera2D& camera, std::vector<Raider>& raiders, bool isRaidActive, std::vector<Farm>& farms, const Resources& res) {
    Vector2 pos = GetWorldToScreen2D(_position, camera);
    if (CheckCollisionPointRec(GetMousePosition(), Rectangle {pos.x, pos.y, _size.x, _size.y}) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (AddResource(1, farms, res)) {
            if (isRaidActive) {
                _knights.push_back(Knight(Vector2{_position.x + _size.x / 2, _position.y + _size.y / 2}, resourceManager.GetGameTexture("knight"), Vector2{_position.x + _size.x / 2, _position.y + _size.y / 2}));
                _knights.back().SetTaskMode(TaskMode::TO_RAIDER);
            } else {
                _knights.push_back(Knight(Vector2{_position.x + _size.x / 2, _position.y + _size.y / 2}, resourceManager.GetGameTexture("knight"), Vector2{_position.x + _size.x / 2, _position.y + _size.y / 2}));
            }
        }

    }
    for (auto& knight: _knights) {
        knight.Update(raiders);
    }
}

void Barrack::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
    for (const auto& knight: _knights) {
        knight.Draw();
    }
}

void Barrack::MobilizeKnights() {
    for (auto& knight: _knights) {
        if (knight.GetTaskMode() == TaskMode::PATROLLING) {
            knight.SetTaskMode(TaskMode::TO_RAIDER);
        }
    }
}

void Barrack::DemobilizeKnights() {
    for (auto& knight: _knights) {
        knight.SetTaskMode(TaskMode::PATROLLING);
    }
}

Worker::Worker(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): MovingGameObject(pos, Vector2 {80.0f, 50.0f}, sprite), _homePosition(homePosition) {};

void Worker::Draw() const {
    if (_taskMode == TaskMode::RESTING)
        DrawText("Zzz...", static_cast<int>(GetPosition().x + 20), static_cast<int>(GetPosition().y), 30, WHITE);
    else {
        DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
        if (_hungry) {
            DrawText("hungry(", static_cast<int>(GetPosition().x), static_cast<int>(GetPosition().y - 20), 20, WHITE);
        }
    }
}

Vector2 Worker::FindClosestObject(const std::vector<Storage*> &storages) {
    Vector2 closestPosition = { -1, -1 };
    float closestDistance = std::numeric_limits<float>::max();

    for (const auto& storage : storages) {
        if (!storage->isFull()) {
            float distance = Vector2Distance(_position, storage->GetPosition());
            if (distance < closestDistance) {
                closestDistance = distance;
                closestPosition = storage->GetPosition();
            }
        }
    }
    return closestPosition;
}

bool Worker::AddResourceToStorage(std::vector<Storage*> &storages) {
    for (auto& storage : storages) {
        if (Vector2Distance(_position, storage->GetPosition()) < _speed) {
            if (storage->AddResource(_resourceAmount)) {
                return true;
            }
        }
    }
    return false;
}


void Worker::DecreaseEnergy(int delta) {
    _energy -= delta;
}

void Worker::SetHomePosition(Vector2 pos) {
    _homePosition = pos;
}

void Worker::Hunger(bool value) {
    _hungry = value;
}


Lumberjack::Lumberjack(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): Worker(pos, sprite, homePosition) {_taskMode = TaskMode::TO_TREE;};

void Lumberjack::Update(std::vector<Storage*>& woodStorages, Map& map) {
    if (_hungry) _taskMode = TaskMode::IDLE;
    switch (_taskMode) {
        case TaskMode::TO_TREE: {
            Vector2 targetTreePosition = map.FindClosestTreePosition(_position);
            MoveForwardTarget(targetTreePosition);
            if (IsAtTarget(targetTreePosition)) _taskMode = TaskMode::COLLECTING;
        }
            break;
        case TaskMode::TO_HOME:
            MoveForwardTarget(_homePosition);
            if (IsAtTarget(_homePosition)) {
                _taskMode = TaskMode::RESTING;
            }
            break;
        case TaskMode::COLLECTING:
            _collectingTime += GetFrameTime();
            if (_collectingTime >= _timeToCollect) {
                _taskMode = TaskMode::DELIVERING;
                _collectingTime = 0.0f;
            }
            break;
        case TaskMode::RESTING:
            _restingTime += GetFrameTime();
            if (_restingTime >= _timeToRest) {
                _taskMode = TaskMode::TO_TREE;
                _energy = _maxEnergy;
                _restingTime = 0.0f;
            }
            break;
        case TaskMode::IDLE:
            if (IsAtTarget(_homePosition)) {
                if (!_hungry) {
//                    if (_energy != _maxEnergy) _restingTime += GetFrameTime();
//                    if (_restingTime >= _timeToRest) {
//                        _energy = _maxEnergy;
//                        _restingTime = 0.0f;
//                    }
                    Vector2 closestStorage = FindClosestObject(woodStorages);
                    if (!IsEqual(closestStorage, Vector2{-1, -1})) {
                        _taskMode = TaskMode::DELIVERING;
                    }
                }
            } else MoveForwardTarget(_homePosition);
            break;
        case TaskMode::DELIVERING:
            if (woodStorages.empty()) {
                _taskMode = TaskMode::IDLE;
                break;
            }
            Vector2 closestStorage = FindClosestObject(woodStorages);
            if (IsEqual(closestStorage, Vector2{-1, -1})) {
                _taskMode = TaskMode::IDLE;
                break;
            }
            MoveForwardTarget(closestStorage);
            if (IsAtTarget(closestStorage)) {
                bool successfullyStored = AddResourceToStorage(woodStorages);
                if (successfullyStored) {
                    DecreaseEnergy();
                    if (_energy <= 0) {
                        _taskMode = TaskMode::TO_HOME;
                    } else {
                        _taskMode = TaskMode::TO_TREE;
                    }
                }
            }
            break;

    }
}

Miner::Miner(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): Worker(pos, sprite, homePosition) {_taskMode = TaskMode::TO_STONE;};

void Miner::Update(std::vector<Storage*>& stoneStorages, Map& map) {
    if (_hungry) _taskMode = TaskMode::IDLE;
    switch (_taskMode) {
        case TaskMode::TO_STONE: {
            Vector2 targetStonePosition = map.FindClosestStonePosition(_position);
            MoveForwardTarget(targetStonePosition);
            if (IsAtTarget(targetStonePosition)) _taskMode = TaskMode::COLLECTING;
        }
            break;
        case TaskMode::TO_HOME:
            MoveForwardTarget(_homePosition);
            if (IsAtTarget(_homePosition)) {
                _taskMode = TaskMode::RESTING;
            }
            break;
        case TaskMode::COLLECTING:
            _collectingTime += GetFrameTime();
            if (_collectingTime >= _timeToCollect) {
                _taskMode = TaskMode::DELIVERING;
                _collectingTime = 0.0f;
            }
            break;
        case TaskMode::RESTING:
            _restingTime += GetFrameTime();
            if (_restingTime >= _timeToRest) {
                _taskMode = TaskMode::TO_STONE;
                _energy = _maxEnergy;
                _restingTime = 0.0f;
            }
            break;
        case TaskMode::IDLE:
            if (IsAtTarget(_homePosition)) {
                if (!_hungry) {
                    Vector2 closestStorage = FindClosestObject(stoneStorages);
                    if (!IsEqual(closestStorage, Vector2{-1, -1})) {
                        _taskMode = TaskMode::DELIVERING;
                    }
                }
            } else MoveForwardTarget(_homePosition);
            break;
        case TaskMode::DELIVERING:
            if (stoneStorages.empty()) {
                _taskMode = TaskMode::IDLE;
                break;
            }
            Vector2 closestStorage = FindClosestObject(stoneStorages);
            if (IsEqual(closestStorage, Vector2{-1, -1})) {
                _taskMode = TaskMode::IDLE;
                break;
            }
            MoveForwardTarget(closestStorage);
            if (IsAtTarget(closestStorage)) {
                bool successfullyStored = AddResourceToStorage(stoneStorages);
                if (successfullyStored) {
                    DecreaseEnergy();
                    if (_energy <= 0) {
                        _taskMode = TaskMode::TO_HOME;
                    } else {
                        _taskMode = TaskMode::TO_STONE;
                    }
                }
            }
            break;

    }
}

Farmer::Farmer(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): Worker(pos, sprite, homePosition) {_taskMode = TaskMode::TO_FARM; _timeToCollect = 6.0f;}

Farm* Farmer::FindClosestFarm(std::vector<Farm>& farms) {
    Farm* closestFarm = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < farms.size(); i++) {
//        std::cout << "POSITION: " << farms[i].GetPosition().x << " " << farms[i].GetPosition().y << " ";
//        std::cout << farms[i].GetCurrentResourceCount() << std::endl;
        std::cout << farms[i].GetFarmersCount() << std::endl;
        if (!farms[i].isFull() && (farms[i].GetFarmersCount() < farms[i].GetMaxFarmersCount()) && !farms[i].IsDestroyed()) {
            float distance = Vector2Distance(_position, farms[i].GetPosition());
            if (distance < closestDistance) {
                closestDistance = distance;
                closestFarm = &farms[i];
            }
        }
    }
    return closestFarm;
}

Farm* Farmer::FindClosestTargetFarm(std::vector<Farm>& farms) {
    Farm* closestFarm = nullptr;

    for (size_t i = 0; i < farms.size(); i++) {
//        std::cout << "POSITION: " << farms[i].GetPosition().x << " " << farms[i].GetPosition().y << " ";
        if (!farms[i].IsDestroyed() && IsEqual(farms[i].GetPosition(), _targetFarmPosition)) {
                closestFarm = &farms[i];
        }
    }
    return closestFarm;
}

// Кароч не удаляются фермеры из фермы когда она заполнена тк ближайшая ферма перестает искаться изза условия заполненностим
void Farmer::Update(std::vector<Farm>& farms, Map& map) {
    if (_hungry && _taskMode != TaskMode::COLLECTING) {
        _taskMode = TaskMode::IDLE;
    }

    switch (_taskMode) {
        case TaskMode::TO_HOME:
            MoveForwardTarget(_homePosition);
            if (IsAtTarget(_homePosition)) {
                _taskMode = TaskMode::RESTING;
            }
            break;
        case TaskMode::COLLECTING: {
//            for (auto& farm : farms) {
//                std::cout << "POSITION: " << farm->GetPosition().x << " " << farm->GetPosition().y << " ";
//            }
//            std::cout << "\n";
            Farm *targetFarm = FindClosestTargetFarm(farms);
            if (targetFarm && !targetFarm->isFull()) {
                if (_hungry) {
                    targetFarm->RemoveFarmer();
                    _collectingTarget = Vector2{-1, -1};
                    _taskMode = TaskMode::IDLE;
                    break;
                }
                if (IsEqual(_collectingTarget, Vector2{-1, -1})) {
                    _collectingTarget = {(float) GetRandomValue(targetFarm->GetPosition().x,
                                                                targetFarm->GetPosition().x + targetFarm->GetSize().x),
                                         (float) GetRandomValue(targetFarm->GetPosition().y,
                                                                targetFarm->GetPosition().y +
                                                                targetFarm->GetSize().y)};
                }
                MoveForwardTarget(_collectingTarget);
                if (IsAtTarget(_collectingTarget)) {
                    _collectingTime += GetFrameTime();
                    if (_collectingTime >= _timeToCollect) {
                        if (targetFarm->AddResource(_resourceAmount)) {
                            DecreaseEnergy();
                            if (_energy <= 0) {
                                targetFarm->RemoveFarmer();
                                _collectingTarget = Vector2{-1, -1};
                                _taskMode = TaskMode::TO_HOME;
                            }
                            _collectingTarget = {(float) GetRandomValue(targetFarm->GetPosition().x,
                                                                        targetFarm->GetPosition().x +
                                                                        targetFarm->GetSize().x),
                                                 (float) GetRandomValue(targetFarm->GetPosition().y,
                                                                        targetFarm->GetPosition().y +
                                                                        targetFarm->GetSize().y)};;
                        } else {
                            targetFarm->RemoveFarmer();
                            _collectingTarget = Vector2{-1, -1};
                            _taskMode = TaskMode::TO_FARM;
                        }
                        _collectingTime = 0.0f;
                    }
                }
            } else {
                if (targetFarm) {
                    targetFarm->RemoveFarmer();
                }
                _collectingTarget = Vector2{-1, -1};
                _targetFarmPosition = Vector2{-1, -1};
                _taskMode = TaskMode::TO_FARM;
            }
            break;
        }
        case TaskMode::RESTING:
            std::cout << "resting\n";
            _restingTime += GetFrameTime();
            if (_restingTime >= _timeToRest) {
                _taskMode = TaskMode::TO_FARM;
                _energy = _maxEnergy;
                _restingTime = 0.0f;
                }
            break;
        case TaskMode::IDLE:
            std::cout << "idle\n";
            if (IsAtTarget(_homePosition)) {
                if (!_hungry) {
                    _taskMode = TaskMode::TO_FARM;
                }
            } else MoveForwardTarget(_homePosition);
            break;
        case TaskMode::TO_FARM:
            Farm* targetFarm = FindClosestFarm(farms);
            if (targetFarm) {
                MoveForwardTarget(targetFarm->GetPosition());
                std::cout << "go to farm\n";
                if (IsAtTarget(targetFarm->GetPosition())) {
                    _taskMode = TaskMode::COLLECTING;
                    _collectingTime = 0.0f;
                    _targetFarmPosition = targetFarm->GetPosition();
                    targetFarm->AddFarmer();
//                std::cout << "am here and added\n";
//                std::cout << "farmPosition: " << farms[_currentFarmInd].GetPosition().x << " " << farms[_currentFarmInd].GetPosition().y << "\n";
                }
            } else {
                _taskMode = TaskMode::IDLE;
            }
            break;
    }
}

Warrior::Warrior(const Vector2 pos, const Texture2D texture): MovingGameObject(pos, Vector2 {50.0f, 50.0f}, texture) {}

bool Warrior::IsAlive() const {
    return _alive;
}

void Warrior::TakeDamage(int damage) {
    _hp -= damage;
    if (_hp <= 0) {
        _alive = false;
    }
    _damageFlashTime = _maxDamageFlashTime;
}

TaskMode &Warrior::GetTaskMode() {
    return _taskMode;
}

void Warrior::Attack(Warrior* target) {
    if (_attackCooldown <= 0.0f) {
        target->TakeDamage(_damage);
        _attackCooldown = _attackCooldownDuration;
    }
}

void Warrior::SetTaskMode(TaskMode task) {
    _taskMode = task;
}

void Warrior::Draw() const {
    if (_damageFlashTime > 0.0f) {
        Color tint = {255, 0, 0, 255};
        DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, tint);
    } else {
        DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
    }
}

Knight::Knight(const Vector2 pos, const Texture2D sprite, const Vector2 barrackPosition): Warrior(pos, sprite), _barrackPosition(barrackPosition) {_taskMode = TaskMode::PATROLLING; _patrolPoint = GetRandomPatrolPoint();}

Vector2 Knight::GetRandomPatrolPoint() {
    float offsetX = GetRandomValue(-100, 100);
    float offsetY = GetRandomValue(-100, 100);
    return Vector2 {_barrackPosition.x + offsetX, _barrackPosition.y + offsetY};
}

Raider* Knight::FindClosestRaider(std::vector<Raider> &raiders) {
    Raider* closestRaider = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < raiders.size(); i++) {
//        std::cout << "POSITION: " << farms[i].GetPosition().x << " " << farms[i].GetPosition().y << " ";
        if (raiders[i].IsAlive()) {
            float distance = Vector2Distance(_position, raiders[i].GetPosition());
            if (distance < closestDistance) {
                closestDistance = distance;
                closestRaider = &raiders[i];
            }
        }
    }
//    std::cout << "\n";
    return closestRaider;

}

void Knight::Update(std::vector<Raider>& raiders) {
    if (_attackCooldown > 0.0f) {
        _attackCooldown -= GetFrameTime();
    }
    switch (_taskMode) {
        case TaskMode::PATROLLING:
            if (IsAtTarget(_patrolPoint)) {
                if (_waitTime > 0.0f) {
                    _waitTime -= GetFrameTime();
                } else {
                    _patrolPoint = GetRandomPatrolPoint();
                    _waitTime = 3.0f;
                }
            } else {
                MoveForwardTarget(_patrolPoint);
            }
            break;
        case TaskMode::TO_RAIDER:
            Raider* nearestRaider = FindClosestRaider(raiders);
            if (nearestRaider) {
                MoveForwardTarget(nearestRaider->GetPosition());
                if (Vector2Distance(_position, nearestRaider->GetPosition()) <= _attackRange && nearestRaider->IsAlive()) {
                    Attack(nearestRaider);
//                    nearestRaider->SetTaskMode(TaskMode::ATTACK);
                }
            }
            break;
    }
}

Raider::Raider(const Vector2 pos, const Texture2D sprite): Warrior(pos, sprite) {_taskMode = TaskMode::DELIVERING; _size = Vector2{30.0f, 30.0f};}

void Raider::Attack(Storage *target) {
    if (_attackCooldown <= 0.0f) {
        target->TakeDamage(_damage);
        _attackCooldown = _attackCooldownDuration;
    }
}

void Raider::Update(std::vector<GameObject*> &allTargets, Map &map) {
    if (_attackCooldown > 0.0f) {
        _attackCooldown -= GetFrameTime();
    }
    if (_damageFlashTime > 0.0f) {
        _damageFlashTime -= GetFrameTime();
    }
    switch (_taskMode) {
//        case TaskMode::ATTACK:
//            Attack(_targetStorage);
//            if (_targetStorage->IsDestroyed()) _taskMode = TaskMode::DELIVERING;
//            break;
        case TaskMode::DELIVERING:
            Vector2 pos = _position;
            auto closestTarget = std::min_element(std::begin(allTargets), std::end(allTargets),
                                                                  [pos](const GameObject* a, const GameObject* b) {
                                                                      return Vector2Distance(a->GetPosition(), pos) < Vector2Distance(b->GetPosition(), pos);
                                                                  });
            Storage* targetStorage = dynamic_cast<Storage*>(*closestTarget);
//            std::cout << target->GetPosition().x << " " << target->GetPosition().y << "\n";
            if (targetStorage) {
                MoveForwardTarget(targetStorage->GetPosition());
                if (IsAtTarget(targetStorage->GetPosition())) {
                    Attack(targetStorage);
                }
            }
            break;
    }
}