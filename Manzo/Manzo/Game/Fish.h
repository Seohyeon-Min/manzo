#ifndef FISH_H
#define FISH_H

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

private:
	double swimming_range = 15.0;
	static int money;
};

#endif