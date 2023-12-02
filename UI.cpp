//
// Created by Григорий on 08.10.2023.
//

#include "UI.h"
#include "iostream"

// shift + enter
// shift + f6 -- rename global
// cmd + down -- go to function
// cmd + shif + move

void UI::Initialize(Vector2 screenSize, ResourceManager& resourceManager, Game* game) {
    _game = game;
    _mainPanel.AddElement(std::make_unique<BuildMenuButton>(screenSize.x - 110, screenSize.y - 110, 100, 100, true, resourceManager.GetGameTexture("build_menu_button"), &_buildMenu));
    _mainPanel.SetVisibility(true);
    _buildMenu.AddElement(std::make_unique<CloseButton>(screenSize.x - 40, screenSize.y - 160, 35, 35, true, resourceManager.GetGameTexture("close_button"), &_buildMenu));
    _buildMenu.AddElement(std::make_unique<BuildCategoryButton>(screenSize.x - 380, screenSize.y - 138, 110, 30, true, resourceManager.GetGameTexture("storage_category_button"), &_storageMenu, &_buildMenu));
    _buildMenu.AddElement(std::make_unique<BuildCategoryButton>(screenSize.x - 490, screenSize.y - 140, 90, 25, true, resourceManager.GetGameTexture("house_category_button"), &_houseMenu, &_buildMenu));
    _buildMenu.AddElement(std::make_unique<BuildCategoryButton>(screenSize.x - 250, screenSize.y - 140, 90, 25, true, resourceManager.GetGameTexture("defence_category_button"), &_defenceMenu, &_buildMenu));
    _buildMenu.LinkPanel(&_storageMenu);
    _buildMenu.LinkPanel(&_houseMenu);
    _buildMenu.LinkPanel(&_defenceMenu);
    _houseMenu.AddElement(std::make_unique<BuildingButton>(screenSize.x - 490, screenSize.y - 100, 70, 70, true, resourceManager.GetGameTexture("lumberjack_house"), BuildingType::LumberjackHouse, _game));
    _houseMenu.AddElement(std::make_unique<BuildingButton>(screenSize.x - 390, screenSize.y - 100, 70, 70, true, resourceManager.GetGameTexture("miner_house"), BuildingType::MinerHouse, _game));
    _houseMenu.AddElement(std::make_unique<BuildingButton>(screenSize.x - 290, screenSize.y - 100, 70, 70, true, resourceManager.GetGameTexture("farmer_house"), BuildingType::FarmerHouse, _game));
    _storageMenu.AddElement(std::make_unique<BuildingButton>(screenSize.x - 490, screenSize.y - 100, 70, 70, true, resourceManager.GetGameTexture("wood_storage"), BuildingType::WoodStorage, _game));
    _storageMenu.AddElement(std::make_unique<BuildingButton>(screenSize.x - 390, screenSize.y - 100, 70, 70, true, resourceManager.GetGameTexture("stone_storage"), BuildingType::StoneStorage, _game));
    _storageMenu.AddElement(std::make_unique<BuildingButton>(screenSize.x - 290, screenSize.y - 100, 70, 70, true, resourceManager.GetGameTexture("farm"), BuildingType::Farm, _game));
    _defenceMenu.AddElement(std::make_unique<BuildingButton>(screenSize.x - 490, screenSize.y - 100, 70, 70, true, resourceManager.GetGameTexture("barrack"), BuildingType::Barrack, _game));

}

void UI::Draw() {
    _mainPanel.Draw();
    _buildMenu.Draw();
    _storageMenu.Draw();
    _houseMenu.Draw();
    _defenceMenu.Draw();
}

void UI::Update() {
    Vector2 mousePosition = GetMousePosition();
    _mainPanel.Update(mousePosition);
    _buildMenu.Update(mousePosition);
    _storageMenu.Update(mousePosition);
    _houseMenu.Update(mousePosition);
    _defenceMenu.Update(mousePosition);
}