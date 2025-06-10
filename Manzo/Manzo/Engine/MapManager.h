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
	Map* GetMap(int index) { return maps[index]; }

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
		valid_spawn_positions.clear();
		mask.clear();
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

	void LoadPNG();
	vec2 MaskToWorld(int maskX, int maskY);
	ivec2 WorldToMask(vec2 worldPos);
	bool IsMaskTrue(vec2 worldPos);
	vec2 Spawn();
	int GetWorldWidth() { return width; }
	int GetWorldHeight() { return height; }
	std::vector<std::vector<bool>> GetMask() { return mask; }

private:
	std::mt19937 gen;

private:
	int read_line_number = -1;

	RockGroup* currentGroup = nullptr;

	vec2 translate = { 0, 0 };
	float rotateAngle = 0;
	vec2 rotatetranslate = { 0, 0 };
	vec2 scale = { 1.0f, 1.0f };

	bool IsinG = false;
	bool IsTranslate = false;
	bool IsRotate = false;
	bool IsScale = false;
	bool IsinGroup = false;
	std::string pathData;
	std::string polyIndex;
	std::string circleIndex;

	bool level_loaded = false;
	std::ifstream file;

	char currentCommand = '\0';
	float margin = 1500.;	// padding for map's partial drawing

	int width, height, channels;
	std::vector<std::vector<bool>> mask;

	std::vector<Polygon> original_polygons;
	std::vector<Polygon> modified_polygons;
	std::vector<Rock*> rocks;
	std::vector<RockGroup*> rock_groups;
	vec2 circle_position{ 0,0 };
	std::vector<ivec2> valid_spawn_positions;

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
	std::regex pathIdRegex;
};