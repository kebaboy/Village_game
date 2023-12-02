//
// Created by Григорий on 02.12.2023.
//

#include "Menu.h"
#include "raylib.h"
#include <iostream>

Menu::Menu(int sW, int sH): _screenWidth(sW), _screenHeight(sH),
                            _startButton(UIButton(_screenWidth/2 - 100.0f, _screenHeight/2 - 75.0f, 200.0f, 150.0f, true, [this]() {_currentMenuState = MenuState::StartGame; std::cout << "start\n";})) {}

void Menu::Initialize(ResourceManager &resourceManager) {
    _startButton.SetTexture(resourceManager.GetGameTexture("start"));
}

void Menu::Draw() {
    BeginDrawing();
    ClearBackground(WHITE);
    _startButton.Draw();
    EndDrawing();
}

void Menu::Update() {
    _startButton.Update();
}

MenuState Menu::GetCurrentState() const {
    return _currentMenuState;
}

void Menu::Reset() {
    _currentMenuState = MenuState::InMenu;
}