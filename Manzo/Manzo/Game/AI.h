#pragma once

#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/MapManager.h"

class BackgroundFish : public CS230::GameObject
{
public:
	BackgroundFish();
	~BackgroundFish() { backgroundFishes.clear(); }
	GameObjectTypes Type() override { return GameObjectTypes::BackgroundFish; }
	std::string TypeName() override { return "Background Fish"; }

	void Update(double dt);
	void Draw();

	void AddBackgroundFishes(BackgroundFish* obj)
	{
		backgroundFishes.push_back(obj);
	}

private:
	vec2 start_position;
	std::list<BackgroundFish*> backgroundFishes;

	float PointToSegmentDistance(const vec2& point, const vec2& segmentStart, const vec2& segmentEnd);
	void HitWithRock(CS230::RectCollision* collision_edge);
	vec2 direction = { 0,0 };

	vec2 GetPerpendicular(vec2 v) {
		return { -v.y, v.x };
	}

};