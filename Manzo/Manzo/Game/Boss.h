#pragma once
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"
#include "BossBullet.h"
#include "JellyEnemy.h"
#include <list>
#include <chrono>
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
		MovingToLocation,
		MovingToLocationPlus
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
	void Bullet(Boss* boss, BossBullet::BulletType type);
	void Jelly(Boss* boss, JellyEnemy::JellyType jellytype, int X_position, int y_posiiton , BossBullet::BulletType bullettype);
	void AttackCircle(vec2 pos, double radius, double elapsed_time);
	void DrawShieldRange(vec2 pos, double radius);
	void UpdateAttackCircles(double dt);
	void Spawn8WayBullets(vec2 position);

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
	static void MovingtolocationPlus_Boss(int targetEntryNum, Boss* object);
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
	bool flipX = false;
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
	bool bulletSpawned = false;
	int barCount;
	bool isattack;
	float currentScaleX = 1.0f;
	float targetScaleX = 1.0f;
	bool jelly_pop = false;

	GLTexture* boss_body = nullptr;
	vec2 position_pro;
	ProceduralChain procedural_jelly;
	ProceduralChain procedural_jelly2;
	std::vector<GLTexture*> texture_vector;
	mat3 matrix_body1;
	mat3 matrix_body2;
	mat3 matrix_tail1;
	mat3 matrix_tail2;
	mat3 matrix_tail3;
	mat3 matrix_tail4;

	bool has_spawned_offset = false;

};

