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
#include "..\Game\GameObjectTypes.h"
#include "..\Game\Rock.h"
#include "..\Game\RockGroup.h"
#include "..\Game\RockPoint.h"

#include <vector>
class Map;

class MapManager : public Component {
public:
	void AddMapFile(const std::string& filename);
	void LoadFirstMap();
	void LoadNextMap();
	void UpdateMaps(const Math::rect& camera_boundary);
	
private:
	std::vector<Map*> maps;
	std::vector<std::string> mapFiles;
	int currentMapIndex = 0;
	float EndY = -100.0f;
};

class Map : public Component {
public:
	~Map() {
		rocks.clear();
		rock_groups.clear();
	}
	void ParseSVG(const std::string& filename);	// SVG file parsing
	std::vector<vec2> parsePathData(const std::string& pathData);	// path parsing
	std::vector<Rock*>& GetRocks() { return rocks; }
	void LoadMapInBoundary(const Math::rect& camera_boundary);// Load map in camera boundary
	bool IsOverlapping(const Math::rect& a, const Math::rect& b);  // Check rock is in camera boundary

	void Translate(const vec2& offset); // Translate map along the offset
	void UnloadAll();

	std::vector<Polygon> Triangulation(std::vector<vec2> positions);


private:
	char currentCommand = '\0';
	float margin = 100.0f;
	std::vector<Rock*> rocks;
	std::vector<RockGroup*> rock_groups;
	vec2 circle_position{ 0,0 };
	float EndY = 100.0f;


};