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
	void AddMapFile(const std::string& filename);
	void LoadFirstMap();
	void LoadNextMap();
	void UpdateMaps(const Math::rect& camera_boundary);
	Map* GetMap(int index) { return maps[index]; }
	std::string GetMapIndex(const std::string& path);

private:
	std::vector<Map*> maps;
	std::vector<std::string> mapFiles;
	int currentMapIndex = 0;
};
