#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "BeatSystem.h"

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
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
	void Move(double dt);
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
	static void LoadBossfile();
	void ReadBossJSON(BossType type);
	void RunMusic();
	void InitializeStates();

private:
	class State_CutScene : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "CutScene"; }
	};

	class Entry1 : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Idle"; }
	};

	class Entry2 : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Idle"; }
	};
	class Entry3 : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Idle"; }
	};
	class Entry4 : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Idle"; }
	};


	State_CutScene state_cutscene;
	Entry1 entry1;
	Entry2 entry2;
	Entry3 entry3;
	Entry4 entry4;

	Beat* beat;
	std::string boss_name;
	int index;
	bool is_boss_fight;
	int bpm;
	std::string mp3_file_name;
	std::array<int, 2> move_position;
	std::vector<std::vector<EntryData>> parttern;
	std::vector<int> total_entry;
	int Boss_damage;
	std::vector<int> Boss_procedural;
	vec2 current_position;
	float speed = 400;
};

