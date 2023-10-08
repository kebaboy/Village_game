//
// Created by Григорий on 08.10.2023.
//

#ifndef VILLAGE_UI_H
#define VILLAGE_UI_H
#include "vector"
#include "memory"
#include "UIElement.h"

class UI {
    std::vector<std::unique_ptr<UIElement>> _uiElements;
public:
    void AddElement(std::unique_ptr<UIElement> element);
    void RemoveElement(/*...*/);
    UIElement* FindElement(/*...*/);
    void DrawAll() const;
    void UpdateAll();
    bool BtnPressed() const;
};


#endif //VILLAGE_UI_H
