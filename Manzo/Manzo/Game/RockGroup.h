/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  RockGroup.ch
Project:    Manzo
Author:     Won Kim
Created:    November 25, 2024
*/
#pragma once

#include "vec2.h"
#include "..\Engine\Component.h"
#include "..\Engine\GameObject.h"
#include "..\Engine\Polygon.h"
#include "..\Engine\MapManager.h"
#include "..\Engine\Polygon.h"
#include "GameObjectTypes.h"
#include "Rock.h"
#include "RockPoint.h"

#include <vector>
class Rock;
class RockPoint;
class RockGroup : public CS230::GameObject //group of polys
{
public:
	RockGroup(const std::string& index);
	~RockGroup() {
		rocks.clear();
	}
	GameObjectTypes Type() override { return GameObjectTypes::RockBoundary; }
	std::string TypeName() override { return "Rock Boundary"; }
	void Update(double dt);
	void Draw();
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision(GameObject* other_object);

	void AddRockPoint(RockPoint* rockpoint) { rock_points.push_back(rockpoint); }
	std::vector<RockPoint*>GetRockPoint() { return rock_points; }

	void AddRock(Rock* rock) { rocks.push_back(rock); }
	std::vector<Rock*> GetRocks() { return rocks; }
	bool MatchIndex();
	vec2 FindCenterRect();
	vec2 FindCenterPoly();
	std::string GetIndex() { return index; }

	// Points
	std::vector<vec2> GetPoints() { return points; }
	void SetPoints();

private:
	bool can_collide = true;
	mat3 matrix;
	RockPoint* rockpoint;
	std::vector<Rock*> rocks;
	std::string index = "";
	std::vector<vec2> points;	// All polygon's points
};
