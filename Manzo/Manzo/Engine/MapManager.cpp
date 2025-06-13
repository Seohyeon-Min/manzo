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

void MapManager::AddMap(Map* new_map) {
    maps.push_back(new_map);
}

void MapManager::LoadMap() {
    if (maps.empty()) return;
    maps[currentMapIndex]->OpenSVG();
    maps[currentMapIndex]->LoadPNG();
}

void MapManager::UpdateMaps(const Math::rect& camera_boundary) {
    
    if (currentMapIndex < maps.size()) {
        maps[currentMapIndex]->LoadMapInBoundary(camera_boundary);

        if (!maps[currentMapIndex]->IsOverlapping(maps[currentMapIndex]->GetMapBoundary(), camera_boundary)) { // Is player  in the level boundary?
            if (!MapIncreased) {
                maps[currentMapIndex]->UnloadAll(); //Unload Previous Map

                if (currentMapIndex + 1 <= maps.size()) {
                    currentMapIndex++;
                    LoadMap();

                }
            }

        }
        else MapIncreased = false;
        
    }
}

