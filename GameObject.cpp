//
// Created by Григорий on 05.10.2023.
//

#include "GameObject.h"
#include "ResourceManager.h"

ResourceManager resourceManager;

GameObject::GameObject(const Vector2 pos): _position(pos) {

}

void GameObject::Draw() {
    DrawTexture(_sprite, _position.x, _position.y, WHITE);
}

void GameObject::Update() {};

Vector2 GameObject::GetPositon() const {
    return _position;
}

void GameObject::SetTexture(const Texture2D &texture) {
    _sprite = texture;
}

Player::Player(const Vector2 pos) : GameObject(pos) {}

void Player::Update() {
    if (IsKeyDown(KEY_RIGHT)) _position.x += _playerVelocity;
    if (IsKeyDown(KEY_LEFT)) _position.x -= _playerVelocity;
    if (IsKeyDown(KEY_UP)) _position.y -= _playerVelocity;
    if (IsKeyDown(KEY_DOWN)) _position.y += _playerVelocity;
}

void Player::Draw() {
    DrawTexturePro(_sprite, Rectangle {0.0f, 0.0f, (float)_sprite.width/4, (float)_sprite.height/4}, Rectangle{_position.x,_position.y, 70, 70}, Vector2{0,0}, 0.0f, WHITE);
}
