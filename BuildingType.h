//
// Created by Григорий on 28.10.2023.
//

#ifndef VILLAGE_BUILDINGTYPE_H
#define VILLAGE_BUILDINGTYPE_H

enum class BuildingType {
    LumberjackHouse,
    MinerHouse,
    FarmerHouse,
    WoodStorage,
    StoneStorage,
    Farm,
    Barrack,
};

struct ResourceInfo {
    std::string resourceName;
    Texture2D resourceImage;
    int resourceValue;
};

#endif //VILLAGE_BUILDINGTYPE_H
