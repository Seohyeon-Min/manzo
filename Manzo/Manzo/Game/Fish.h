#ifndef FISH_H
#define FISH_H

#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "../Engine/MapManager.h"

class Fish : public GameObject
{
public:
	Fish() : Fish(nullptr) { };
	Fish(Fish* parent);
	GameObjectTypes Type() override { return GameObjectTypes::Fish; }
	std::string TypeName() override { return "Fish"; }
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
	static void ReadFishCSV(const std::string& filename);
	int GetMoney() { return money; }
	void ClearMoney() { money = 0; }
	void SetMoney(int count) { money = count; }

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
	void Draw();
	vec2 AvoidRock(vec2 thisPos, vec2 rockPos);
	bool IsRockInfront(vec2 thisPos, vec2 rockPos);

private:
	static int money;
	bool IsAvoided = false;
	float angleRadians;
	double coolTime = 1.5f;
	Fish* parentFish = nullptr;
	vec2 findRock;
	vec2 AvoidanceVelocity;
	bool AvoidanceActive = false;
};

#endif