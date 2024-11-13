#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/MapManager.h"

class Fish : public CS230::GameObject
{
public:
	Fish() : Fish(nullptr) { };
	Fish(Fish* parent);
	GameObjectTypes Type() override { return GameObjectTypes::Fish; }
	std::string TypeName() override { return "Fish"; }
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
	static void ReadFishCSV(const std::string& filename);

	vec2 start_position = { 0,0 };

	enum FishType
	{
		Fish1 = 1, Fish2, Fish3
	};

	struct FishDex
	{
		FishType type;
		vec2 scale;
		vec2 velocity;
		std::filesystem::path filePath;
	};

	static std::vector<FishDex> fishBook;
	int type = FishType::Fish1;

	bool collided = false;

	void Update(double dt);
	void Draw(DrawLayer drawlayer)override;

private:
	double swimming_range = 15.0;
};

class BackgroundFish : public CS230::GameObject
{
public:
	BackgroundFish();
	~BackgroundFish() { backgroundFishes.clear(); }
	GameObjectTypes Type() override { return GameObjectTypes::BackgroundFish; }
	std::string TypeName() override { return "Background Fish"; }
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

	void Update(double dt);
	void Draw(DrawLayer drawlayer)override;

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