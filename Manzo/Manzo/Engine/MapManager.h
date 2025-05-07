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
#include "..\Game\Box.h"


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
	
private:
	std::vector<Map*> maps;
	std::vector<std::string> mapFiles;
	int currentMapIndex = 0;
	float EndY = -5600.0f;
};

class Map : public Component {
public:
	Map();
	~Map() {
		rocks.clear();
		rock_groups.clear();
	}

	void OpenSVG(const std::string& filename);
	void ParseSVG();
	std::vector<vec2> parsePathData(const std::string& pathData);	// path parsing
	void LoadMapInBoundary(const Math::rect& camera_boundary);
	bool IsOverlapping(const Math::rect& a, const Math::rect& b);
	
	void SetMargin(float margin) { this->margin = margin; }
	float GetMargin() { return margin; }
	
	void UnloadAll();
	bool IsLevelLoaded() { return level_loaded; }


private:
	int read_line_number = -1;

	RockGroup* currentGroup = nullptr;
	std::string currentTag;

	vec2 translate = { 0, 0 };
	float rotateAngle = 0;
	vec2 rotatetranslate = { 0, 0 };
	vec2 scale = { 1.0f, 1.0f };

	bool IsinG = false;
	bool IsTranslate = false;
	bool IsRotate = false;
	bool IsScale = false;
	bool IsinGroup = false;
	std::string polyIndex;
	std::string circleIndex;

	bool level_loaded = false;
	std::ifstream file;

	char currentCommand = '\0';
	float margin = 1500.f;

	std::vector<Polygon> original_polygons;
	std::vector<Polygon> modified_polygons;
	std::vector<Rock*> rocks;
	std::vector<RockGroup*> rock_groups;
	vec2 circle_position{ 0,0 };
	float EndY = -7000.0f;

	// SVG parsing tags
	std::regex pathRegex;
	std::regex gIdRegex;
	std::regex circleRegex;
	std::regex cyRegex;
	std::regex cIdxRegex;
	std::regex labelRegex;
	std::regex transformRegex;
	std::regex translateRegex;
	std::regex rotateRegex;
	std::regex matrixRegex;
	std::regex svgCloseRegex;
};