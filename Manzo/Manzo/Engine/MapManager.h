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

namespace CS230 {
	class Map : public CS230::Component {
	public:
		~Map() {
			objects.clear();
			rock_groups.clear();
		}
		void ParseSVG(const std::string& filename);
		void AddDrawCall();

		std::vector<Rock> GetRock()
		{
			return objects;
		}

	private:
		//std::vector<RockPoint*> rock_points;
		std::vector<Rock> objects;
		std::vector<RockGroup*> rock_groups;
		vec2 circle_position{ 0,0 };

	};
}
