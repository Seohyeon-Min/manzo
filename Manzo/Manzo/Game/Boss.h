#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "BeatSystem.h"

#include <vector>

class Boss : public GameObject
{
public:
	enum BossName //for jsonfile read
	{
		e,
		y,
		count
	};

	enum class BossType {
		ChasingPlayer,   
		Shooting,        
		MultiInstance,   
		MovingToLocation 
	};

	Boss(vec2 start_position, BossName name, BossType type);
	GameObjectTypes Type() override { return GameObjectTypes::Boss; }
	std::string TypeName() override { return "Boss"; }
	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
	void Move(double dt);
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
	static void LoadBossfile();
	void ReadBossJSON(BossName type);
	void RunMusic();
	void InitializeStates();
	void AfterDied();
	const std::array<int, 2> GetFirstPosition() { return position; }


private:

	//class BossBehavior {
	//public:
	//	virtual void Update(Boss* boss, double dt) = 0; // Abstract method
	//	virtual void Initialize(Boss* boss) = 0;        // For setup
	//	virtual ~BossBehavior() = default;
	//};

	//void InitializeBehavior(BossType type); // Factory Method
	
	static void Check_BossBehavior(int targetEntryNum, GameObject* object);
	//------
	static void Movingtolocation_Boss(int targetEntryNum, Boss* object);
	static void Chasingplayer_Boss(int targetEntryNum, Boss* object);
	static void Shooting_Boss(int targetEntryNum, Boss* object);
	static void MultiInstance_Boss(int targetEntryNum, Boss* object);

	BossType bossType;
	//std::unique_ptr<BossBehavior> behavior; // Polymorphic behavior


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
		std::string GetName() override { return "Entry1"; }
	};

	class Entry2 : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Entry2"; }
	};
	class Entry3 : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Entry3"; }
	};
	class Entry4 : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Entry4"; }
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
	std::array<int, 2> position;
	std::vector<std::vector<EntryData>> parttern;
	std::vector<int> total_entry;
	int Boss_damage;
	std::vector<int> Boss_procedural;
	vec2 current_position;
	float speed = 400;
	int channelId = 1;
	bool isPlaying = false;
	std::array<int, 2> start_pos;
};

