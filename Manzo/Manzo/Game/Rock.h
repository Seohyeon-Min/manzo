/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Rock.h
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
#include "RockGroup.h"

#include <vector>

class RockGroup;
class Rock : public CS230::GameObject
{
public:
	Rock(Polygon poly);
	~Rock() {};
	GameObjectTypes Type() override { return GameObjectTypes::Rock; }
	std::string TypeName() override { return "Rock"; }
	void Update(double dt);
	void Draw();
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision(GameObject* other_object);

	//position & moving
	void SetCenter();
	vec2 Normalize(const vec2& vec);
	bool IsRange(vec2& current_position);
	void Pop(const vec2& direction, float speed);
	void PopBack(const vec2& direction, float speed);

	//polygon
	const Polygon& GetPolygon() { return poly; }

	//group
	void SetRockGroup(RockGroup* rockgroup) { this->rockgroup = rockgroup; }
	RockGroup* GetRockGroup() { return rockgroup; }
private:
	float range = 4;
	vec2 start_position;
	RockGroup* rockgroup;
	Polygon poly;
};
