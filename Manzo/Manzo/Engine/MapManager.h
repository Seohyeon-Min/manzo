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

class Map : public Component {
public:
	~Map() {
		objects.clear();
		rock_groups.clear();
	}
	void ParseSVG(const std::string& filename);
	std::vector<vec2> parsePathData(const std::string& pathData);	// path parsing
	std::vector<Rock>& GetRocks() { return objects; }
	void MakeRockGroups(Rock* rock, Polygon poly);
	void MakeMovingRockGroups(MovingRock* moving_rock, Polygon poly);
	void AddDrawCall();


private:
	char currentCommand = '\0';
	std::vector<Rock> objects;
	std::vector<RockGroup*> rock_groups;
	vec2 circle_position{ 0,0 };


};