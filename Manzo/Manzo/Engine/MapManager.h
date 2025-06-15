/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  MapManager.h
Project:    Manzo
Author:     SeokWha Hong
Created:    September 12, 2024
*/

#pragma once

#include "vec2.h"
#include "Component.h"
#include "GameObject.h"
#include "Polygon.h"
#include "..\Game\Map.h"


#include <vector>
#include <regex>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cmath>
#include <algorithm>

class Map;

class MapManager : public Component {
public:
	Map* GetCurrentMap();
	void AddMap(Map* new_map);
	void LoadMap();
	void UpdateMaps(const Math::rect& camera_boundary);
	Map* GetMap(int index) { return maps[index]; }
	bool IsMapChanged() { return MapChanged; }

	void Unload();

private:
	std::vector<Map*> maps;
	int currentMapIndex = 0;
	bool MapIncreased = false;
	bool MapChanged = false;
};