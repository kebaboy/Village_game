//
// Created by Григорий on 05.10.2023.
//

#include "GameObject.h"
#include "ResourceManager.h"
#include "iostream"

ResourceManager resourceManager;

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

Player::Player(const Vector2 pos, const Vector2 size) : GameObject(pos, size) {}

void Player::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width/4, (float)_sprite.height/4}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

const float Player::GetVelocity() const {
    return _playerVelocity;
}

WorkerHouse::WorkerHouse(const Vector2 pos, const Texture2D sprite): GameObject(pos, Vector2{100, 100}, sprite) {}

//void House::DrawOpacity() const {
//    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{GetMousePosition().x, GetMousePosition().y, _size.x, _size.y}, Vector2{0,0}, 0.0f, Fade(WHITE, 0.5f));
//}

LumberjackHouse::LumberjackHouse(const Vector2 pos, const Texture2D sprite): WorkerHouse(pos, sprite) {}

int LumberjackHouse::GetCostFood() const {return 10;}
int LumberjackHouse::GetCostWood() const {return 70;}
int LumberjackHouse::GetCostStone() const {return 50;}

MinerHouse::MinerHouse(const Vector2 pos, const Texture2D sprite): WorkerHouse(pos, sprite) {}

int MinerHouse::GetCostFood() const {return 10;}
int MinerHouse::GetCostWood() const {return 50;}
int MinerHouse::GetCostStone() const {return 70;}

FarmerHouse::FarmerHouse(const Vector2 pos, const Texture2D sprite): WorkerHouse(pos, sprite) {}

int FarmerHouse::GetCostFood() const {return 10;}
int FarmerHouse::GetCostWood() const {return 50;}
int FarmerHouse::GetCostStone() const {return 70;}


Storage::Storage(const Vector2 pos, const Texture2D sprite): GameObject(pos, Vector2{150.0f, 150.0f}, sprite) {}

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

WoodStorage::WoodStorage(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {}

StoneStorage::StoneStorage(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {}

Farm::Farm(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {}

int Farm::GetFarmersCount() const {
    return _farmersCount;
}

int Farm::GetMaxFarmersCount() const {
    return _maxFarmersCount;
}

bool Farm::AddFarmer() {
    _farmersCount++;
}

bool Farm::RemoveFarmer() {
    _farmersCount--;
}

Tree::Tree(const Vector2 pos, const Vector2 size, const Texture2D sprite): GameObject(pos, size, sprite) {}



Worker::Worker(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): GameObject(pos, Vector2 {80.0f, 50.0f}, sprite), _homePosition(homePosition) {};

Vector2 Worker::FindClosestStorage(const std::vector<Storage*> &storages) {
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

void Worker::MoveForwardTarget(const Vector2 target) {
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

bool Worker::IsAtTarget(const Vector2 target) {
    if (Vector2Distance(_position, target) < _speed) {
        return true;
    }
    return false;
}

void Worker::DecreaseEnergy(int delta) {
    _energy -= delta;
}


Lumberjack::Lumberjack(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): Worker(pos, sprite, homePosition) ,_taskMode(TaskMode::TO_TREE) {};

void Lumberjack::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

void Lumberjack::SetHomePosition(Vector2 pos) {
    _homePosition = pos;
}

void Lumberjack::Update(std::vector<Storage*>& woodStorages, Map& map) {
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
            _choppingTime += GetFrameTime();
            if (_choppingTime >= _timeToChop) {
                _taskMode = TaskMode::DELIVERING;
                _choppingTime = 0.0f;
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
                if (_energy != _maxEnergy) _restingTime += GetFrameTime();
                if (_restingTime >= _timeToRest) {
                    _energy = _maxEnergy;
                    _restingTime = 0.0f;
                }
                Vector2 closestStorage = FindClosestStorage(woodStorages);
                if (!IsEqual(closestStorage, Vector2{-1, -1})) {
                    _taskMode = TaskMode::DELIVERING;
                }
            } else MoveForwardTarget(_homePosition);
            break;
        case TaskMode::DELIVERING:
            if (woodStorages.empty()) {
                _taskMode = TaskMode::IDLE;
                break;
            }
            Vector2 closestStorage = FindClosestStorage(woodStorages);
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

Miner::Miner(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): Worker(pos, sprite, homePosition) ,_taskMode(TaskMode::TO_STONE) {};

void Miner::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

void Miner::SetHomePosition(Vector2 pos) {
    _homePosition = pos;
}

void Miner::Update(std::vector<Storage*>& stoneStorages, Map& map) {
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
            _minningTime += GetFrameTime();
            if (_minningTime >= _timeToMine) {
                _taskMode = TaskMode::DELIVERING;
                _minningTime = 0.0f;
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
                if (_energy != _maxEnergy) _restingTime += GetFrameTime();
                if (_restingTime >= _timeToRest) {
                    _energy = _maxEnergy;
                    _restingTime = 0.0f;
                }
                Vector2 closestStorage = FindClosestStorage(stoneStorages);
                if (!IsEqual(closestStorage, Vector2{-1, -1})) {
                    _taskMode = TaskMode::DELIVERING;
                }
            } else MoveForwardTarget(_homePosition);
            break;
        case TaskMode::DELIVERING:
            if (stoneStorages.empty()) {
                _taskMode = TaskMode::IDLE;
                break;
            }
            Vector2 closestStorage = FindClosestStorage(stoneStorages);
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

Farmer::Farmer(const Vector2 pos, const Texture2D sprite, const Vector2 homePosition): Worker(pos, sprite, homePosition), _taskMode(TaskMode::TO_FARM) {}

void Farmer::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

void Farmer::SetHomePosition(Vector2 pos) {
    _homePosition = pos;
}

void Farmer::FindClosestFarm(std::vector<Farm>& farms) {
    int closestFarmInd = -1;
    float closestDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < farms.size(); i++) {
//        std::cout << "POSITION: " << farms[i].GetPosition().x << " " << farms[i].GetPosition().y << " ";
        if (!farms[i].isFull() && (farms[i].GetFarmersCount() < farms[i].GetMaxFarmersCount())) {
            float distance = Vector2Distance(_position, farms[i].GetPosition());
            if (distance < closestDistance) {
                closestDistance = distance;
                closestFarmInd = i;
            }
        }
    }
//    std::cout << "\n";
    _currentFarmInd = closestFarmInd;
}

void Farmer::Update(std::vector<Farm>& farms, Map& map) {
    switch (_taskMode) {
        case TaskMode::TO_FARM:
            if (farms.empty()) {
//                std::cout << "yes\n";
                _taskMode = TaskMode::IDLE;
                break;
            }
            FindClosestFarm(farms);
//            std::cout << _currentFarmInd << "\n";
            if (_currentFarmInd == -1) {
//                std::cout << "yes2\n";
                _taskMode = TaskMode::IDLE;
                break;
            }
            MoveForwardTarget(farms[_currentFarmInd].GetPosition());
//            std::cout << "go to farm\n";
            if (IsAtTarget(farms[_currentFarmInd].GetPosition())) {
                _taskMode = TaskMode::COLLECTING;
                farms[_currentFarmInd].AddFarmer();
//                std::cout << "am here and added\n";
//                std::cout << "farmPosition: " << farms[_currentFarmInd].GetPosition().x << " " << farms[_currentFarmInd].GetPosition().y << "\n";
            }
            break;
        case TaskMode::TO_HOME:
            MoveForwardTarget(_homePosition);
            if (IsAtTarget(_homePosition)) {
                _taskMode = TaskMode::RESTING;
            }
            break;
        case TaskMode::COLLECTING:
//            for (auto& farm : farms) {
//                std::cout << "POSITION: " << farm->GetPosition().x << " " << farm->GetPosition().y << " ";
//            }
//            std::cout << "\n";
            if (farms[_currentFarmInd].isFull()) {
                _collectingTarget = Vector2{-1, -1};
                if (_currentFarmInd != -1) {
                    farms[_currentFarmInd].RemoveFarmer();
                    _currentFarmInd = -1;
                }
                _taskMode = TaskMode::TO_FARM;
            } else {
                if (IsEqual(_collectingTarget, Vector2{-1, -1})) {
                    _collectingTarget = {(float)GetRandomValue(farms[_currentFarmInd].GetPosition().x, farms[_currentFarmInd].GetPosition().x + farms[_currentFarmInd].GetSize().x), (float)GetRandomValue(farms[_currentFarmInd].GetPosition().y, farms[_currentFarmInd].GetPosition().y + farms[_currentFarmInd].GetSize().y)};;
                }
                MoveForwardTarget(_collectingTarget);
                if (IsAtTarget(_collectingTarget)) {
                    _harvestingTime += GetFrameTime();
                    if (_harvestingTime >= _timeToHarvest) {
                        if (farms[_currentFarmInd].AddResource(_resourceAmount)) {
                            DecreaseEnergy();
                            if (_energy <= 0) {
                                if (_currentFarmInd != -1) {
                                    farms[_currentFarmInd].RemoveFarmer();
                                    _currentFarmInd = -1;
                                }
                                _collectingTarget = Vector2{-1, -1};
                                _taskMode = TaskMode::TO_HOME;
                            }
                            _collectingTarget = {(float)GetRandomValue(farms[_currentFarmInd].GetPosition().x, farms[_currentFarmInd].GetPosition().x + farms[_currentFarmInd].GetSize().x), (float)GetRandomValue(farms[_currentFarmInd].GetPosition().y, farms[_currentFarmInd].GetPosition().y + farms[_currentFarmInd].GetSize().y)};
                        }
                        _harvestingTime = 0.0f;
                    }
                }
            }
            break;
        case TaskMode::RESTING:
            _restingTime += GetFrameTime();
            if (_restingTime >= _timeToRest) {
                _taskMode = TaskMode::TO_FARM;
                _energy = _maxEnergy;
                _restingTime = 0.0f;
            }
            break;
        case TaskMode::IDLE:
            if (IsAtTarget(_homePosition)) {
                if (_energy != _maxEnergy) _restingTime += GetFrameTime();
                if (_restingTime >= _timeToRest) {
                    _energy = _maxEnergy;
                    _restingTime = 0.0f;
                }
                _taskMode = TaskMode::TO_FARM;
            } else MoveForwardTarget(_homePosition);
            break;
    }
}