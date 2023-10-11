//
// Created by Григорий on 08.10.2023.
//

#ifndef VILLAGE_UI_H
#define VILLAGE_UI_H
#include "vector"
#include "memory"
#include "UIElement.h"
#include "ResourceManager.h"

class UI {
private:
    std::vector<std::unique_ptr<UIElement>> _uiElements;
public:
    void Initialize(Vector2 screenSize, ResourceManager& resourceManager);
    void AddElement(std::unique_ptr<UIElement> element);
    void RemoveElement(/*...*/);
    UIElement* GetElement(size_t id);
    void DrawAll() const;
    void UpdateAll();
//    bool BuildMenuBtnPressed() const;
};


#endif //VILLAGE_UI_H
