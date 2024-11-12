#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include <vector>


class Boss : public CS230::GameObject
{
public:
	enum BossType
	{
		e,
		count
	};

	Boss(vec2 start_position, BossType type);
	GameObjectTypes Type() override { return GameObjectTypes::Boss; }
	std::string TypeName() override { return "Boss"; }
	void Update(double dt) override;
	void Draw() override;
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
	static void LoadBossfile();
	void ReadBossJSON(BossType type);

	

private:

	std::string boss_name;
	int index;
	bool is_boss_fight;
	int bpm;
	std::string mp3;
	std::array<int, 2> move_position;
	std::vector<std::vector<EntryData>> parttern;
	std::vector<int> total_entry;
	int Boss_damage;
	std::vector<int> Boss_procedural;
};

