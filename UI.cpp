//
// Created by Григорий on 08.10.2023.
//

#include "UI.h"
#include "iostream"

void UI::Initialize(Vector2 screenSize, ResourceManager& resourceManager) {
//    AddElement(std::make_unique<UIButton>(screenSize.x -100, screenSize.y - 400, 70, 70, false, resourceManager.GetGameTexture("start_btn"),"Click me!"));
    AddElement(std::make_unique<BuildMenuBtn>(screenSize.x -150, screenSize.y - 100, 130, 70, true, resourceManager.GetGameTexture("start_btn")));
    // shift + enter
    // shift + f6 -- rename global
    // cmd + down -- go to function
    // cmd + shif + move
    AddElement(std::make_unique<BuildMenu>(screenSize.x -90, screenSize.y - 420, 70, 300, false, resourceManager.GetGameTexture("house")));
    BuildMenu* pbuildMenu = static_cast<BuildMenu*>(_uiElements[1].get());
    pbuildMenu->AddBuildMenuElement(std::make_unique<UIButton>(screenSize.x - 90, screenSize.y - 420, 70, 70, false, resourceManager.GetGameTexture("house"),"Click me!"));
    pbuildMenu->AddBuildMenuElement(std::make_unique<UIButton>(screenSize.x - 90, screenSize.y - 340, 70, 70, false, resourceManager.GetGameTexture("wood_storage"),"Click me!"));
}

void UI::AddElement(std::unique_ptr<UIElement> element) {
    _uiElements.push_back(std::move(element));
}

UIElement* UI::GetElement(size_t id) {
    //dynamic_cast<UIButton*>(element.get())
    return _uiElements[id].get();
}

void UI::DrawAll() const {
    for (const auto& elem : _uiElements) {
        elem->Draw();
    }
}

void UI::UpdateAll() {
    if (_uiElements[0]->BtnPressed()) {
//        _uiElements[0]->SetVisibility(true);
        _uiElements[1]->SetVisibility(true);
    } else _uiElements[1]->SetVisibility(false);
    for (const auto& elem : _uiElements) {
        elem->Update();
    }
}

//bool UI::BuildMenuBtnPressed() const {
//    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(),
//                                                                          Rectangle{_uiElements[0]->GetPosition().x,
//                                                                                    _uiElements[0]->GetPosition().y,
//                                                                                    _uiElements[0]->GetSize().x,
//                                                                                    _uiElements[0]->GetSize().y})) {
//        std::cout << "Btn Pressed\n";
//        return true;
//    }
//    return false;
//}