//
// Created by Григорий on 05.10.2023.
//

#include "GameObject.h"
#include "ResourceManager.h"

ResourceManager resourceManager;

GameObject::GameObject(const Vector2 pos, const Vector2 size): _position(pos), _size(size) {}

GameObject::GameObject(const Vector2 pos, const Vector2 size, const Texture2D sprite): _position(pos), _size(size),
                                                                                       _sprite(sprite) {}

void GameObject::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

void GameObject::Update() {};

const Vector2& GameObject::GetPositon() const {
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


Storage::Storage(const Vector2 pos, const Texture2D sprite): GameObject(pos, Vector2{100.0f, 100.0f}, sprite) {}

bool Storage::AddResource(int amount) {
    if (_resourceCount + amount <= _maxCapacity) {
        _resourceCount += amount;
        return true;
    }
    return false;
}

int Storage::GetCurrentResourceCount() const {
    return _resourceCount;
}

WoodStorage::WoodStorage(const Vector2 pos, const Texture2D sprite): Storage(pos, sprite) {}



Tree::Tree(const Vector2 pos, const Vector2 size, const Texture2D sprite): GameObject(pos, size, sprite) {}




Lumberjack::Lumberjack(const Vector2 pos, const Vector2 size, const Texture2D sprite): GameObject(pos, size, sprite), _housePosition(pos), _taskMode(LumberjackTaskMode::TO_TREE) {};

void Lumberjack::Draw() const {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x,_position.y, _size.x, _size.y}, Vector2{0,0}, 0.0f, WHITE);
}

void Lumberjack::SetTargetTreePosition(Vector2 pos) {
    _targetTreePosition = pos;
}

void Lumberjack::Update(int& woodCount) {
    Vector2 direction;
    float magnitude;

    switch (_taskMode) {
        case LumberjackTaskMode::CHOPPING:
            _choppingTime += GetFrameTime();
            if (_choppingTime >= _timeToChop) {
                woodCount += _choppingAmount;
                _taskMode = LumberjackTaskMode::DELIVERING;
                _targetTreePosition = _housePosition;
                _choppingTime = 0.0f;
            }
            break;

        case LumberjackTaskMode::DELIVERING:


        case LumberjackTaskMode::TO_TREE:
        case LumberjackTaskMode::TO_HOME:
            direction = {
                    _targetTreePosition.x - _position.x,
                    _targetTreePosition.y - _position.y
            };

            magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
            direction.x /= magnitude;
            direction.y /= magnitude;

            _position.x += direction.x * _speed;
            _position.y += direction.y * _speed;

            if (Vector2Distance(_position, _targetTreePosition) < _speed) {
                _position = _targetTreePosition;
                if (_taskMode == LumberjackTaskMode::TO_TREE) {
                    _taskMode = LumberjackTaskMode::CHOPPING;
                    _currentTree = _targetTreePosition;
                    _targetTreePosition = _housePosition; // двигаемся обратно к начальной позиции
                } else {
                    _taskMode = LumberjackTaskMode::TO_TREE;
                    _targetTreePosition = _currentTree;
                }
            }
            break;

        default:
            // Обработка неизвестного режима задачи, если таковой имеется.
            break;
    }
}