//
// Created by Григорий on 08.10.2023.
//

#include "UI.h"
#include "iostream"

void UI::AddElement(std::unique_ptr<UIElement> element) {
    _uiElements.push_back(std::move(element));
}

void UI::DrawAll() const {
    for (const auto& elem : _uiElements) {
        elem->Draw();
    }
}

void UI::UpdateAll() {

}

bool UI::BtnPressed() const {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(),
                                                                          Rectangle{_uiElements[0]->GetPosition().x,
                                                                                    _uiElements[0]->GetPosition().y,
                                                                                    _uiElements[0]->GetSize().x,
                                                                                    _uiElements[0]->GetSize().y})) {
        std::cout << "Btn Pressed\n";
        return true;
    }
    return false;
}