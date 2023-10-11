//
// Created by Григорий on 05.10.2023.
//

#include "GameObject.h"
#include "ResourceManager.h"

ResourceManager resourceManager;

GameObject::GameObject(const Vector2 pos, const Vector2 size): _position(pos), _size(size) {
}

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

Building::Building(const Vector2 pos, const Vector2 size, const Texture2D sprite): GameObject(pos, size) {_sprite = sprite;}

//void House::DrawOpacity() const {
//    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{GetMousePosition().x, GetMousePosition().y, _size.x, _size.y}, Vector2{0,0}, 0.0f, Fade(WHITE, 0.5f));
//}
Tree::Tree(const Vector2 pos, const Vector2 size, const Texture2D sprite): GameObject(pos, size) {_sprite = sprite;}