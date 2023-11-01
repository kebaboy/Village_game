//
// Created by Григорий on 08.10.2023.
//

#include "UIElement.h"
#include "Game.h"
#include "iostream"

UIElement::UIElement(float x, float y, float w, float h, bool visibility, Texture2D texture)
        : _position({x, y}), _size({w, h}), _visible(visibility), _sprite(texture) {}

        UIElement::UIElement(float x, float y, float w, float h, bool visibility)
        : _position({x, y}), _size({w, h}), _visible(visibility) {}

void UIElement::Draw() {
    if (!_visible) return;
    DrawTexturePro(_sprite, Rectangle{0.0f, 0.0f, (float)_sprite.width, (float)_sprite.height}, Rectangle{_position.x, _position.y, _size.x, _size.y}, Vector2{0, 0}, 0.0f, WHITE);
}

void UIElement::Update() {

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

Texture2D UIElement::GetTexture() const {
    return _sprite;
}

void UIElement::SetPosition(float x, float y) {
    _position = {x, y};
}




UIButton::UIButton(float x, float y, float w, float h, bool visibility, Texture2D texture, std::function<void()> onClick): UIElement(x, y, w, h, visibility, texture), _onClickFunc(onClick) {}

void UIButton::Update() {
    if (CheckCollisionPointRec(GetMousePosition(),Rectangle {_position.x, _position.y, _size.x, _size.y}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        OnClick();
    }
}

void UIButton::OnClick() {
    if (_onClickFunc) {
        _onClickFunc();
    }
}

BuildMenuButton::BuildMenuButton(float x, float y, float w, float h, bool visibility, Texture2D texture, UIPanel* panel): UIButton(x, y, w, h, visibility, texture, [this]() {this->OpenMenu();}), _buildMenuPanel(panel) {}

void BuildMenuButton::OpenMenu() {
    if (_buildMenuPanel) {
        _buildMenuPanel->SetVisibility(true);
    }
}

void UIPanel::AddElement(std::unique_ptr<UIButton> element) {
    _elements.push_back(std::move(element));
}

void UIPanel::SetVisibility(bool value) {
    _visible = value;
    for (auto& elem : _elements) {
        elem->SetVisibility(value);
    }
}

void UIPanel::Update(const Vector2& mousePosition) {
    if (!_visible) return;
    for (auto& elem : _elements) {
        elem->Update();
    }
}

void UIPanel::Draw() {
    if (!_visible) return;
    for (auto& elem : _elements) {
        elem->Draw();
    }
}

UIElement* UIPanel::GetElement(size_t id) {
    //dynamic_cast<UIButton*>(element.get())
    return _elements[id].get();
}

void BuildMenuPanel::HideAllCategoriesExcept(UIButton *visiblePanel) {
    for (auto& btn : _elements) {  // Assuming _categoryPanels is a list of all such panels
        if (btn.get() != visiblePanel) {
            BuildCategoryButton* categoryButton = dynamic_cast<BuildCategoryButton*>(btn.get());
            if (categoryButton) {
                categoryButton->GetCategory()->SetVisibility(false);
            }
        }
    }
}

void BuildMenuPanel::Draw() {
    if (!_visible) return;
    DrawRectangle(GetScreenWidth() - 500, GetScreenHeight() - 150, 490, 140, Color(BROWN));
    for (auto& elem : _elements) {
        elem->Draw();
    }
}

void BuildMenuPanel::SetVisibility(bool value) {
    _visible = value;
    for (auto& elem : _elements) {
        elem->SetVisibility(value);
    }
    if (!value) {
        for (auto panel : _linkedPanels) {
            panel->SetVisibility(false);
        }
    }
}
void BuildMenuPanel::LinkPanel(UIPanel *panel) {
    _linkedPanels.push_back(panel);
}

BuildCategoryButton::BuildCategoryButton(float x, float y, float w, float h, bool visibility, Texture2D texture,
                                         UIPanel *panel, BuildMenuPanel* parent): UIButton(x, y, w, h, visibility, texture, [this]() {this->OpenCategory();}), _categoryPanel(panel), _parentUI(parent) {}

void BuildCategoryButton::OpenCategory() {
    if (_categoryPanel) {
        _categoryPanel->SetVisibility(true);
        _parentUI->HideAllCategoriesExcept(this);
    }
}

UIPanel* BuildCategoryButton::GetCategory() const {
    return _categoryPanel;
}

void BuildCategoryButton::SetVisibility(bool value) {
    _visible = value;
    _categoryPanel->SetVisibility(value);
}

BuildingButton::BuildingButton(float x, float y, float w, float h, bool visibility, Texture2D texture, BuildingType type, Game* game): UIButton(x, y, w, h, visibility, texture, [this]() {this->Build();}), _buildingType(type), _game(game){}

BuildingType BuildingButton::GetBuildingType() const {
    return _buildingType;
}
void BuildingButton::Build() {
    _game->ToggleBuildingPlacementMode(_buildingType);
};

CloseButton::CloseButton(float x, float y, float w, float h, bool visibility, Texture2D texture,
                         UIPanel *panelToClose): UIButton(x, y, w, h, visibility, texture, [panelToClose]() {panelToClose->SetVisibility(
        false);}) {}