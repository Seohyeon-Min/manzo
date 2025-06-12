/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  MapManager.cpp
Project:    Manzo
Author:     SeokWha Hong
Created:    September 12, 2024
*/

#include "stb_image.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "vec2.h"
#include "Collision.h"


#ifndef M_PI
#define M_PI 3.14
#endif


//MapManager

Map* MapManager::GetCurrentMap() {
    if (maps.empty()) return nullptr;
    return maps[currentMapIndex];
}

void MapManager::AddMapFile(const std::string& filename) {
    mapFiles.push_back(filename);
}

void MapManager::LoadFirstMap() {
    if (mapFiles.empty()) return;
    

    Map* initialMap = new Map(GetMapIndex(mapFiles[currentMapIndex]));
    initialMap->OpenSVG(mapFiles[currentMapIndex]);
    initialMap->LoadPNG();
    maps.push_back(initialMap);
}

void MapManager::LoadNextMap() {

    currentMapIndex++;

    Map* nextMap = new Map(GetMapIndex(mapFiles[currentMapIndex]));
    //nextMap->SetMargin(800.0f);
    nextMap->OpenSVG(mapFiles[currentMapIndex]);    //open next svg file

    maps.push_back(nextMap);
}

void MapManager::UpdateMaps(const Math::rect& camera_boundary) {
    if (currentMapIndex < maps.size()) {
        maps[currentMapIndex]->LoadMapInBoundary(camera_boundary);

        if (camera_boundary.Bottom() <= -6000.f) {
            //Unload Previous Map
            maps[currentMapIndex]->UnloadAll();
            
            if (currentMapIndex + 1 <= maps.size()) {
                LoadNextMap();
                if (!maps[currentMapIndex]->IsLevelLoaded()) {    // if next map is not loaded
                    maps[currentMapIndex]->ParseSVG();            //parse SVG file
                }
            }
        }
    }
}

std::string MapManager::GetMapIndex(const std::string& path) {
    size_t start = path.find_last_of('/') + 1;
    size_t end = path.find_last_of('.');
    return path.substr(start, end - start);
}
