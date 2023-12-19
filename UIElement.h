//
// Created by Григорий on 08.10.2023.
//


#ifndef VILLAGE_UIELEMENT_H
#define VILLAGE_UIELEMENT_H
#include "raylib.h"
#include "string"
#include "BuildingType.h"
#include "map"
class Game;

class UIElement {
protected:
    Vector2 _position;
    Vector2 _size;
    bool _visible;
    Texture2D _sprite;
public:
    UIElement(float x, float y, float w, float h, bool visibility, Texture2D texture);
    UIElement(float x, float y, float w, float h, bool visibility);

    virtual void Draw(); // Отрисовка элемента UI
    virtual void Update(); // Обновление логики элемента

    void SetVisibility(bool value);
    bool IsVisible() const;
    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    Texture2D GetTexture() const;
    void SetTexture(Texture2D& texture);
    void SetPosition(float x, float y);
};

class UIButton : public UIElement {
protected:
    std::function<void()> _onClickFunc;
public:
    UIButton(float x, float y, float w, float h, bool visibility, Texture2D texture, std::function<void()> onClick);
    UIButton(float x, float y, float w, float h, bool visibility, std::function<void()> onClick);

    void Update() override;

    void OnClick();
};

class UIPanel {
protected:
    bool _visible = false;
    std::vector<std::unique_ptr<UIButton>> _elements;
public:
    virtual void AddElement(std::unique_ptr<UIButton> element);
    UIElement* GetElement(size_t id);
    void Update(const Vector2& mousePosition);
    virtual void Draw();
    virtual void SetVisibility(bool value);
    bool GetVisibility() const;
};

class BuildMenuButton : public UIButton {
private:
    UIPanel* _buildMenuPanel;
public:
    BuildMenuButton(float x, float y, float w, float h, bool visibility, Texture2D texture, UIPanel* panel);
    void OpenMenu();
};

class BuildMenuPanel: public UIPanel {
private:
    std::vector<UIPanel*> _linkedPanels;
public:
    void HideAllCategoriesExcept(UIButton* visiblePanel);
    void LinkPanel(UIPanel* panel);
    void Draw() override;
    void SetVisibility(bool value) override;
};

class BuildCategoryPanel: public UIPanel {
public:
    void Draw(std::map<BuildingType, std::vector<ResourceInfo>>& resources);
};

class BuildCategoryButton: public UIButton {
private:
    UIPanel* _categoryPanel;
    BuildMenuPanel* _parentUI;
public:
    BuildCategoryButton(float x, float y, float w, float h, bool visibility, Texture2D texture, UIPanel* panel, BuildMenuPanel* parent);
    void OpenCategory();
    UIPanel* GetCategory() const;
    void SetVisibility(bool value);
    void Draw() override;
};

class BuildingButton: public UIButton {
private:
    BuildingType _buildingType;
    Game* _game;
public:
    BuildingButton(float x, float y, float w, float h, bool visibility, Texture2D texture, BuildingType type, Game* game);
    BuildingType GetBuildingType() const;
    void Build();
    void Draw(std::map<BuildingType, std::vector<ResourceInfo>>& resources);
};

class CloseButton: public UIButton {
public:
    CloseButton(float x, float y, float w, float h, bool visibility, Texture2D texture, UIPanel* panelToClose);
};
#endif //VILLAGE_UIELEMENT_H
