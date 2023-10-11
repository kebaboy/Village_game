//
// Created by Григорий on 08.10.2023.
//

#include "UIElement.h"

UIElement::UIElement(float x, float y, float w, float h, bool visibility, Texture2D texture)
        : _position({x, y}), _size({w, h}), _visible(visibility), _sprite(texture) {}

void UIElement::Draw() {
    if (IsVisible()) {
        DrawTexturePro(_sprite, Rectangle{0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x, _position.y, _size.x, _size.y}, Vector2{0, 0}, 0.0f, WHITE);
    }
}

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

bool UIElement::BtnPressed() {
    return _pressed;
}

bool UIElement::BtnUnpressed() {
    _pressed = !_pressed;
}

UIButton::UIButton(float x, float y, float w, float h, bool visibility, Texture2D texture, const std::string& txt)
        : UIElement(x, y, w, h, visibility, texture), _text(txt) {}

void UIButton::Update() {
    if (IsVisible() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(),
                                                                          Rectangle {_position.x, _position.y, _size.x, _size.y})) {
        _pressed = !_pressed;
    }
}

//void UIButton::Draw() {
//    if (IsVisible()) {
//        DrawRectangle(_position.x, _position.y, _size.x, _size.y, BROWN);
//    }
//}

BuildMenuBtn::BuildMenuBtn(float x, float y, float w, float h, bool visibility, Texture2D texture): UIElement(x, y, w, h, visibility, texture) {}

//void BuildMenuBtn::Draw() {
//    if (IsVisible()) {
//        DrawRectangle(_position.x, _position.y, _size.x, _size.y, BROWN);
//    }
//}

void BuildMenuBtn::Update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(),
                                                                          Rectangle {_position.x, _position.y, _size.x, _size.y})) {
        _pressed = !_pressed;
    }
}
BuildMenu::BuildMenu(float x, float y, float w, float h, bool visibility, Texture2D texture): UIElement(x, y, w, h, visibility, texture) {}

void BuildMenu::AddBuildMenuElement(std::unique_ptr<UIElement> element) {
    _elements.push_back(std::move(element));
}

void BuildMenu::Update() {
    if (_visible) {
        for (auto& elem : _elements) {
            elem->SetVisibility(true);
            elem->Update();
        }
    } else {
        for (auto& elem : _elements) {
            elem->SetVisibility(false);
        }
    }
}

void BuildMenu::Draw() {
    if (_visible) DrawRectangle(_position.x, _position.y, _size.x, _size.y, BROWN);
    for (auto& elem : _elements) {
        elem->Draw();
    }
}

UIElement* BuildMenu::GetBuildMenuElement(size_t id) {
    //dynamic_cast<UIButton*>(element.get())
    return _elements[id].get();
}