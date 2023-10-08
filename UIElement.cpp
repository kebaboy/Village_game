//
// Created by Григорий on 08.10.2023.
//

#include "UIElement.h"

UIElement::UIElement(float x, float y, float w, float h)
        : _position({x, y}), _size({w, h}), _visible(true) {}

void UIElement::SetVisibility(bool value) {
    _visible = value;
}

bool UIElement::IsVisible() const {
    return _visible;
}

Vector2 UIElement::GetPosition() const {
    return _position;
}

Vector2 UIElement::GetSize() const {
    return _size;
}

void UIElement::SetPosition(float x, float y) {
    _position = {x, y};
}

UIButton::UIButton(float x, float y, float w, float h, const std::string& txt)
        : UIElement(x, y, w, h), text(txt) {}

void UIButton::Draw() {
    if (IsVisible()) {
        DrawRectangle(_position.x, _position.y, _size.x, _size.y, BROWN);
    }
}