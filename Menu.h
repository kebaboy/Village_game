//
// Created by Григорий on 02.12.2023.
//

#ifndef VILLAGE_MENU_H
#define VILLAGE_MENU_H
#include "UI.h"

enum MenuState {
    InMenu,
    StartGame,
};

class Menu {
private:
    MenuState _currentMenuState = MenuState::InMenu;
    int _screenWidth;
    int _screenHeight;
    Texture2D _gameTitle;
    Texture2D _background;
    UIButton _startButton;
public:
    Menu(int sW, int sH);
    void Initialize(ResourceManager& resourceManager);
    void Draw();
    void Update();
    void Reset();

    MenuState GetCurrentState() const;
};


#endif //VILLAGE_MENU_H
