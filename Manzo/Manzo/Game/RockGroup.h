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
class ObstacleRock;
class MovingRock;
class RockPoint;

class RockGroup : public GameObject
{
public:
	RockGroup(const std::string& index, const std::string& map_index, double rotation, vec2 scale);
	~RockGroup();
	GameObjectTypes Type() override { return GameObjectTypes::RockBoundary; }
	std::string TypeName() override { return "Rock Boundary"; }
	void Update(double dt);
	void Draw();
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision(GameObject* other_object);

	void AddRockPoint(RockPoint* rockpoint) { this->rockpoint = rockpoint; }
	void AddObstacleRock(ObstacleRock* obstacle_rock) { obstacle_rocks.push_back(obstacle_rock); }
	RockPoint* GetRockPoint() { return rockpoint; }

	void AddRock(Rock* rock) { rocks.push_back(rock); }
	void AddMovingRock(MovingRock* moving_rock) { moving_rocks.push_back(moving_rock); }
	std::vector<Rock*> GetRocks() { return rocks; }
	std::vector<MovingRock*> GetMovingRocks() { return moving_rocks; }
	vec2 FindCenterRect();
	vec2 FindCenterPoly();
	Math::rect FindBoundary();
	std::string GetIndex() { return index; }

	void Active(bool active) { loaded = active; }
	bool IsActivated() { return loaded; }

	// Points
	std::vector<vec2> GetPoints() { return points; }
	void SetPoints();	// Fill vector of polygon's points

	//Crash
	void Crash(bool crashed) { this->crashed = crashed; }
	bool IsCrashed() { return crashed; }

private:
	bool crashed = false;
	bool loaded = false; // Is Rockgroup loaded in map?
	bool can_collide = true;
	vec2 start_position;
	mat3 matrix;
	RockPoint* rockpoint = nullptr;
	std::vector<Rock*> rocks;
	std::vector<ObstacleRock*> obstacle_rocks;
	std::vector<MovingRock*> moving_rocks;
	std::string index = "";
	std::string map_index = "";
	std::vector<vec2> points;	// All polygon's points
};
