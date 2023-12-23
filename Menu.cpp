//
// Created by Григорий on 02.12.2023.
//

#include "Menu.h"
#include "raylib.h"
#include <iostream>

Menu::Menu(int sW, int sH): _screenWidth(sW), _screenHeight(sH),
                            _startButton(UIButton(_screenWidth/2 - 125.0f, _screenHeight/2 + 75.0f, 250.0f, 100.0f, true, [this]() {_currentMenuState = MenuState::StartGame; std::cout << "start\n";})) {}

void Menu::Initialize(ResourceManager &resourceManager) {
    _startButton.SetTexture(resourceManager.GetGameTexture("start"));
    _gameTitle = resourceManager.GetGameTexture("village");
    _background = resourceManager.GetGameTexture("menu-background");
}

void Menu::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexturePro(_background, Rectangle {0.0f, 0.0f, (float)_background.width, (float)_background.height}, Rectangle{0.0f, 0.0f, (float)_screenWidth, (float)_screenHeight}, Vector2{0,0}, 0.0f, WHITE);
    _startButton.Draw();
    DrawTexturePro(_gameTitle, Rectangle {0.0f, 0.0f, (float)_gameTitle.width, (float)_gameTitle.height}, Rectangle{_screenWidth / 2 - 400.0f, 100.0f, 800.0f, 120.0f}, Vector2{0,0}, 0.0f, WHITE);
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