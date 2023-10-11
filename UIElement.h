//
// Created by Григорий on 08.10.2023.
//

#ifndef VILLAGE_UIELEMENT_H
#define VILLAGE_UIELEMENT_H
#include "raylib.h"
#include "string"

class UIElement {
protected:
    Vector2 _position;
    Vector2 _size;
    bool _visible;
    bool _pressed = false;
    Texture2D _sprite;
public:
    UIElement(float x, float y, float w, float h, bool visibility, Texture2D texture);

    virtual void Draw(); // Отрисовка элемента UI
    virtual void Update() = 0; // Обновление логики элемента

    void SetVisibility(bool value);
    bool IsVisible() const;
    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    void SetPosition(float x, float y);
    bool BtnPressed();
    bool BtnUnpressed();
};

class UIButton : public UIElement {
private:
    std::string _text;
public:
    UIButton(float x, float y, float w, float h, bool visibility, Texture2D texture, const std::string& txt);

//    void Draw() override;
    void Update() override;

    // Другие утилиты и методы для UIButton...
};

class BuildMenuBtn : public UIElement {
public:
    BuildMenuBtn(float x, float y, float w, float h, bool visibility, Texture2D texture);
//    void Draw() override;
    void Update() override;
};

class BuildMenu: public UIElement {
private:
    std::vector<std::unique_ptr<UIElement>> _elements;
public:
    BuildMenu(float x, float y, float w, float h, bool visibility, Texture2D texture);
    void AddBuildMenuElement(std::unique_ptr<UIElement> element);
    UIElement* GetBuildMenuElement(size_t id);
    void Update() override;
    void Draw() override;
};


#endif //VILLAGE_UIELEMENT_H
