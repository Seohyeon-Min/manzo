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
class MovingRock;
class RockPoint;

class RockGroup : public GameObject
{
public:
	RockGroup(const std::string& index);
	~RockGroup();
	GameObjectTypes Type() override { return GameObjectTypes::RockBoundary; }
	std::string TypeName() override { return "Rock Boundary"; }
	void Update(double dt);
	void Draw();
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision(GameObject* other_object);

	void AddRockPoint(RockPoint* rockpoint) { this->rockpoint = rockpoint; }
	RockPoint* GetRockPoint() { return rockpoint; }

	void AddRock(Rock* rock) { rocks.push_back(rock); }
	void AddMovingRock(MovingRock* moving_rock) { moving_rocks.push_back(moving_rock); }
	std::vector<Rock*> GetRocks() { return rocks; }
	std::vector<MovingRock*> GetMovingRocks() { return moving_rocks; }
	bool MatchIndex();
	vec2 FindCenterRect();
	vec2 FindCenterPoly();
	std::string GetIndex() { return index; }

	// Points
	std::vector<vec2> GetPoints() { return points; }
	void SetPoints();

private:
	bool can_collide = true;
	vec2 start_position;
	mat3 matrix;
	RockPoint* rockpoint = nullptr;
	std::vector<Rock*> rocks;
	std::vector<MovingRock*> moving_rocks;
	std::string index = "";
	std::vector<vec2> points;	// All polygon's points
};
