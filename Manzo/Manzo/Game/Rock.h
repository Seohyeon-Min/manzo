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

#include "..\Engine\MapManager.h"
#include "..\Engine\Component.h"
#include "..\Engine\GameObject.h"
#include "..\Engine\Polygon.h"
#include "..\Engine\Polygon.h"
#include "GameObjectTypes.h"
#include "RockGroup.h"
#include "vec2.h"

#include <vector>

class RockGroup;

class Rock : public GameObject
{
public:
	Rock(Polygon original_poly, Polygon modified_poly, vec2 position, double rotation, vec2 scale);
	~Rock();
	GameObjectTypes Type() override { return GameObjectTypes::Rock; }
	std::string TypeName() override { return "Rock"; }
	void Update(double dt);
	void Draw();

	//position & moving
	void SetCenter();

	//polygon
	const Polygon& GetOriginalPoly() { return original_poly; }
	const Polygon& GetModifiedPoly() { return modified_poly; }

	//group
	void SetRockGroup(RockGroup* rockgroup) { this->rockgroup = rockgroup; }
	RockGroup* GetRockGroup() { return rockgroup; }

	// Map Loading
	void Active(bool active) { loaded = active; }
	bool IsActivated() { return loaded; }

	// Crash
	void Crash(bool crashed) { this->crashed = crashed; }
	bool IsCrashed() { return crashed; }

	std::vector<vec2> GetPoints() { return original_poly.vertices; }

private:
	std::string index;
	RockGroup* rockgroup = nullptr;
	Polygon original_poly;
	Polygon modified_poly;
	bool loaded = false;
	bool crashed = false;
};


class ObstacleRock : public Rock
{
public:
	ObstacleRock(Polygon original_poly, Polygon modified_poly, vec2 position, double rotation, vec2 scale);
	
	GameObjectTypes Type() override { return GameObjectTypes::ObstacleRock; }
	std::string TypeName() override { return "ObstacleRock"; }
	

	//group
	void SetRockGroup(RockGroup* rockgroup) { this->rockgroup = rockgroup; }
	RockGroup* GetRockGroup() { return rockgroup; }

	// Map Loading
	void Active(bool active) { loaded = active; }
	bool IsActivated() { return loaded; }

	std::vector<vec2> GetPoints() { return original_poly.vertices; }

private:
	std::string index;
	RockGroup* rockgroup;
	Polygon original_poly;
	Polygon modified_poly;
	bool loaded = false;
};


class MovingRock : public Rock {
public:
	MovingRock(Polygon original_poly, Polygon modified_poly, vec2 position, double rotation, vec2 scale);
	~MovingRock() {};
	GameObjectTypes Type() override { return GameObjectTypes::MovingRock; }
	std::string TypeName() override { return "Rock"; }
	
	//position & moving
	void Hit(bool hit) { this->hit = hit; }
	bool IsRange(const vec2& current_position);
	void Pop(const vec2& direction, float speed);
	void PopBack(const vec2& direction, float speed);


private:
	bool hit = false;
	float pop_speed = 0.f;	//no delayyyyyyyyyy
	float pop_back_speed = 0.f;
	float range = 50.f;
	RockGroup* rockgroup;
	Polygon poly;

	class State_Idle : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "State_Idle"; }
	};

	class State_Pop : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "State_Pop"; }
	};

	class State_PopBack : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "State_PopBack"; }
	};

	State_Idle state_idle;
	State_Pop state_pop;
	State_PopBack state_popback;
};
