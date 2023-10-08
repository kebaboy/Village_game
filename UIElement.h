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
public:
    UIElement(float x, float y, float w, float h);

    virtual void Draw() = 0; // Отрисовка элемента UI
//    virtual void Update() = 0; // Обновление логики элемента

    void SetVisibility(bool value);
    bool IsVisible() const;
    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    void SetPosition(float x, float y);
};

class UIButton : public UIElement {
private:
    std::string text;
public:
    UIButton(float x, float y, float w, float h, const std::string& txt);

    void Draw() override;
    std::function<void()> onClick;  // Callback
//    void Update() override;

    // Другие утилиты и методы для UIButton...
};


#endif //VILLAGE_UIELEMENT_H
