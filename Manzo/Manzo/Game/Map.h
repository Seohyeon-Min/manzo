/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Map.h
Project:    Manzo
Author:     Won Kim
Created:    June 12, 2025
*/

#pragma once

#include "vec2.h"
#include "..\Engine\Component.h"
#include "..\Engine\GameObject.h"
#include "..\Engine\Polygon.h"
#include "GameObjectTypes.h"
#include "Rock.h"
#include "RockGroup.h"
#include "RockPoint.h"
#include "Box.h"


#include <vector>
#include <regex>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <cmath>
#include <algorithm>

class Map : public Component {
public:
	Map(const std::string& filename, Math::rect map_boundary);

	~Map() {
		rocks.clear();
		rock_groups.clear();
		valid_spawn_positions.clear();
		mask.clear();
	}

	void OpenSVG();
	void ParseSVG();

	std::vector<vec2> parsePathData(const std::string& pathData);	// path parsing
	void LoadMapInBoundary(const Math::rect& camera_boundary);
	bool IsOverlapping(const Math::rect& a, const Math::rect& b);
	bool IsOverlappingMargin(const Math::rect& a, const Math::rect& b);
	bool IsLevelLoaded() { return level_loaded; }
	Math::rect GetMapBoundary() { return map_boundary; }

	void UnloadAll();
	void UnloadCrashedRock();

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
	float margin = 1000.f;	//map drawing zone margin - should be smaller!!!!!
	Math::rect map_boundary = { {0, 0},{0, 0} };
	// mask
	int width, height, channels;
	std::vector<std::vector<bool>> mask;
	std::vector<ivec2> valid_spawn_positions;

	// file reading
	bool level_loaded = false;	// boolean - is this level fully loaded?
	std::ifstream map_file;
	std::string file_path;
	std::string map_index = "";
	int read_line_number = -1;
	char currentCommand = '\0';

	// map objects management
	RockGroup* currentGroup = nullptr;
	std::vector<Polygon> original_polygons;
	std::vector<Polygon> modified_polygons;
	std::vector<Rock*> rocks;
	std::vector<ObstacleRock*> obstacle_rocks;
	std::vector<RockGroup*> rock_groups;
	vec2 circle_position{ 0,0 };

	// SVG parsing index and variables
	vec2 translate = { 0, 0 };
	float rotateAngle = 0;
	vec2 rotatetranslate = { 0, 0 };
	vec2 scale = { 1.0f, 1.0f };
	std::string fillColor = "#000000";	//parsing data's fillcolor
	std::string obstacleColor = "#ba22ff"; // color for obstacle rock
	

	bool IsinG = false;
	bool IsTranslate = false;
	bool IsRotate = false;
	bool IsScale = false;
	bool IsinGroup = false;
	std::string pathData;
	std::string polyIndex;
	std::string circleIndex;

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
	std::regex fillColorRegex;
	std::regex styleRegex;
};