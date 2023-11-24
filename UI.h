//
// Created by Григорий on 08.10.2023.
//

#ifndef VILLAGE_UI_H
#define VILLAGE_UI_H
#include "vector"
#include "memory"
#include "UIElement.h"
#include "ResourceManager.h"
class Game;

class UI {
private:
    UIPanel _mainPanel;
    BuildMenuPanel _buildMenu;
    BuildCategoryPanel _storageMenu;
    BuildCategoryPanel _houseMenu;
    BuildCategoryPanel _defenceMenu;

    Game* _game;
public:
    void Initialize(Vector2 screenSize, ResourceManager& resourceManager, Game* game);
    void AddElement(std::unique_ptr<UIElement> element);
    void RemoveElement(/*...*/);
    UIElement* GetElement(size_t id);
    void Draw();
    void Update();
};

#endif //VILLAGE_UI_H
