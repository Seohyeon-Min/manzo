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
class Map;

class MapManager : public Component {
public:
	void AddMapFile(const std::string& filename);
	void LoadFirstMap();
	void LoadNextMap();
	void UpdateMaps(const Math::rect& camera_boundary);
	Map* GetMap(int index) { return maps[index]; }

private:
	std::vector<Map*> maps;
	std::vector<std::string> mapFiles;
	int currentMapIndex = 0;
	float EndY = -100.0f;
};

class Map : public Component {
public:
	Map()
	{
		std::random_device rd;
		gen = std::mt19937(rd());
	}

	~Map() {
		rocks.clear();
		rock_groups.clear();
		valid_spawn_positions.clear();
		mask.clear();
	}

	void ParseSVG(const std::string& filename);
	std::vector<vec2> parsePathData(const std::string& pathData);	// path parsing
	void LoadMapInBoundary(const Math::rect& camera_boundary);
	bool IsOverlapping(const Math::rect& a, const Math::rect& b);

	void FillPolygonScanline(const std::vector<ivec2>& polygon, std::vector<unsigned char>& data, int width, int height);

	void DrawRockLine(ivec2 p0, ivec2 p1, std::vector<unsigned char>& data, int width, int height);

	void Translate(const vec2& offset);
	void UnloadAll();

	void LoadPNG();
	vec2 MaskToWorld(int maskX, int maskY);
	ivec2 WorldToMask(vec2 worldPos);
	bool IsMaskTrue(vec2 worldPos);
	vec2 Spawn();
	int GetWorldWidth() { return width; }
	int GetWorldHeight() { return height; }
	std::vector<std::vector<bool>> GetMask() { return mask; }

	std::vector<unsigned char> data;
	GLuint obstacleTex = 0;

private:
	std::mt19937 gen;

	char currentCommand = '\0';
	float margin = 2000.f;

	int width, height, channels;
	std::vector<std::vector<bool>> mask;

	std::vector<Polygon> original_polygons;
	std::vector<Polygon> modified_polygons;
	std::vector<Rock*> rocks;
	std::vector<RockGroup*> rock_groups;
	vec2 circle_position{ 0,0 };
	float EndY = 100.0f;
	std::vector<ivec2> valid_spawn_positions;
	

};