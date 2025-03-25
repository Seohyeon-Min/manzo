#include "../Engine/Rapidjson.h"
#include "../Engine/AudioManager.h"
#include "Boss.h"
#include "Ship.h"


std::vector<GameObject::State*> stateMap;
std::vector<std::string> BossJSONfileMap;

Boss::Boss(vec2 start_position, BossName name, BossType type)
	: GameObject(start_position), bossType(type)
{
	ReadBossJSON(name);
	InitializeStates();
	AddGOComponent(new Sprite("assets/images/boss_E.spt", this));
	SetVelocity({ start_position });
	current_position = start_position;
	// cutscean

	Engine::GetAudioManager().LoadMusic(mp3_file_name, "e boss", false, false);
	////////
	current_state = &state_cutscene;
	current_state->Enter(this);

}

void Boss::Movingtolocation_Boss(int targetEntryNum, Boss* object) {
	if (targetEntryNum - 1 < object->parttern.size()) {
		const auto& entryVec = object->parttern[targetEntryNum - 1];
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == object->beat->GetDelayCount()) {
				object->current_position = entryData.position;
			}
		}
	}
}
void Boss::Chasingplayer_Boss(int targetEntryNum, Boss* boss) {

	if (targetEntryNum - 1 < boss->parttern.size()) {
		const auto& entryVec = boss->parttern[targetEntryNum - 1];
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == boss->beat->GetDelayCount()) {
				Ship* ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
				if (ship == nullptr) {
					Engine::GetLogger().LogError("Ship component not found");
					return;
				}

				vec2 playerPosition = ship->GetPosition();
				vec2 bossPosition = boss->GetPosition();
				vec2 directionToPlayer = playerPosition - bossPosition;

				float distanceToPlayer = directionToPlayer.Length();
				if (distanceToPlayer > (boss->speed / 4)) {
					directionToPlayer = directionToPlayer.Normalize();
					boss->current_position = bossPosition + (directionToPlayer * (boss->speed / 4));
				}
				else {
					boss->current_position = playerPosition;
				}

				/*std::cout << "Boss is moving towards player. New position: ("
					<< boss->current_position.x << ", " << boss->current_position.y << ")" << std::endl;*/
			}
		}
	}
}

void Boss::Shooting_Boss(int targetEntryNum, Boss* object) {

}
void Boss::MultiInstance_Boss(int targetEntryNum, Boss* object) {

}

void Boss::Check_BossBehavior(int targetEntryNum, GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);

	switch (boss->bossType)
	{
	case Boss::BossType::MovingToLocation:
		Movingtolocation_Boss(targetEntryNum, boss);
		break;
	case Boss::BossType::ChasingPlayer:
		Chasingplayer_Boss(targetEntryNum, boss);
		break;
	case Boss::BossType::Shooting:
		Shooting_Boss(targetEntryNum, boss);
		break;
	case Boss::BossType::MultiInstance:
		MultiInstance_Boss(targetEntryNum, boss);
		break;

	default:
		Engine::GetLogger().LogError("TYPE ERROR : There is no boss type like that... check one more time!");
		break;
	}
}

void Boss::State_CutScene::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
	boss->beat = Engine::GetGameStateManager().GetGSComponent<Beat>();

}
void Boss::State_CutScene::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::State_CutScene::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);

	if (Engine::GetInput().KeyDown(Input::Keys::R) && boss->beat->GetBeat()) {
		Engine::GetAudioManager().SetMute("background1", true);
		Engine::GetAudioManager().StopChannel("e morse");
		Engine::GetAudioManager().PlayMusics("e boss");

		boss->beat->SetBPM(boss->bpm);
		std::cout << "boss bpm:" << boss->bpm << std::endl;
		boss->change_state(&boss->entry1);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry1::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry1::Update(GameObject* object, double dt) {
	int targetEntryNum = 1;
}
void Boss::Entry1::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
/// /////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry2::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry2::Update(GameObject* object, double dt) {
	int targetEntryNum = 2;
	Check_BossBehavior(targetEntryNum, object);
}
void Boss::Entry2::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
////////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry3::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry3::Update(GameObject* object, double dt) {
	int targetEntryNum = 3;
	Check_BossBehavior(targetEntryNum, object);
}
void Boss::Entry3::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
////////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry4::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry4::Update(GameObject* object, double dt) {
	int targetEntryNum = 4;
	Check_BossBehavior(targetEntryNum, object);
}
void Boss::Entry4::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
////////////////////////////////////////////////////////////////////////////////////////////



void Boss::InitializeStates() {
	stateMap.push_back(&entry1);
	stateMap.push_back(&entry2);
	stateMap.push_back(&entry3);
	stateMap.push_back(&entry4);
}



void Boss::Update(double dt) {
	Boss* boss = static_cast<Boss*>(this);

	if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
		if (GameObject::current_state->GetName() != Boss::state_cutscene.GetName()) {

			int barCount = beat->GetBarCount();
			std::cout << barCount << std::endl;
			if (barCount < 14) {
				if (barCount < total_entry.size() && total_entry[barCount] - 1 < stateMap.size()) {
					change_state(stateMap[total_entry[barCount] - 1]);
				}
				else {

					std::cerr << "Invalid barCount or index out of range: " << barCount << std::endl;
					Destroy();
					AfterDied();
				}
			}
		}

		Move(dt);
	}
}

void Boss::AfterDied()
{
	Engine::GetAudioManager().StopChannel("e boss");
	Engine::GetGameStateManager().GetGSComponent<Beat>()->CleartoOriginal();

	auto pump = Engine::GetGameStateManager().GetGSComponent<Pump>();
	if (pump) {
		pump->Reset();
	}
	Engine::GetAudioManager().RestartPlayMusic("background1");
	Engine::GetAudioManager().SetMute("background1", false);
}

void Boss::AttackCircle(vec2 pos, double radius, double elapsed_time, double attack_interval, double dt)
{
	static double last_attack_time = 0.0;  
	static bool is_attacking = false; 
	static double attack_duration = 0.0;  // attack duration
	static double wait_time = 0.0;  // wait after finishing attack
	shield_range = pos;

	last_attack_time += dt;  // update

	if (is_attacking || (!is_attacking && wait_time == 0.0))
	{
		DrawShieldRange(shield_range, radius);
	}

	// start attack
	if (last_attack_time >= attack_interval && !is_attacking && wait_time == 0.0)
	{
		is_attacking = true;
		attack_duration = attack_interval;  // attack duration (time)
		std::cout << "Attack Started!" << std::endl;
	}

	if (is_attacking)
	{
		attack_duration -= dt;

		vec2 player_pos = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>()->GetPosition();
		double distance = sqrt(pow(player_pos.x - pos.x, 2) + pow(player_pos.y - pos.y, 2)) * 2.0;

		if (distance > radius)
		{
			Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>()->DeclineFuel(0.4);
			std::cout << "Fuel: " << Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>()->GetFuel() << std::endl;
		}

		// finish attack -> wait
		if (attack_duration <= 0.0)
		{
			is_attacking = false;  
			wait_time = elapsed_time;  // set waiting time
			std::cout << "Attack Finished" << std::endl;
		}
	}

	// wait~
	if (!is_attacking && wait_time > 0.0)
	{
		wait_time -= dt;  // decrease waiting time before restart
		std::cout << "Waiting for next attack: " << wait_time << std::endl;

		// restart
		if (wait_time <= 0.0)
		{
			last_attack_time = 0.0;  // ready to attack again
			std::cout << "Ready for next attack." << std::endl;
			wait_time = 0.0;
		}
	}
}

void Boss::DrawShieldRange(vec2 pos, double radius)
{
	CircleDrawCall draw_call = {
		(float)radius,
		{shield_range.x,shield_range.y},
	};

	draw_call.settings.do_blending = true;
	draw_call.sorting_layer = DrawLayer::DrawUI;
	Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call));
}


vec2 Lerp(const vec2& start, const vec2& end, float t) {
	return start + t * (end - start);
}

void Boss::Move(double dt) {
	Boss* boss = static_cast<Boss*>(this);

	vec2 direction = current_position - GetPosition();
	direction = direction.Normalize();

	vec2 force = direction * speed;

	static float lerp_factor = 0.0f;
	lerp_factor += (float)dt * 0.1f;
	lerp_factor = std::min(lerp_factor, 5.0f);

	vec2 lerped_position = Lerp(GetPosition(), current_position, lerp_factor);

	SetVelocity((lerped_position - GetPosition()) / (float)dt);
	GameObjectManager* gameobjectmanager = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>();
	GameObject::Update(dt);
	vec2 nearestRockpoint = gameobjectmanager->FindNearestRock(boss);

	if ((current_position - GetPosition()).Length() < 10.0f) {
		lerp_factor = 0.0f;
	}
}



void Boss::LoadBossfile() {
	BossJSONfileMap.push_back("assets/jsons/boss/boss_e.json");
	BossJSONfileMap.push_back("assets/jsons/boss/boss_y.json");
	BossJSONfileMap.push_back("Please add file path here");
}

void Boss::ReadBossJSON(BossName name)
{
	JsonParser_boss* ReadJson = new JsonParser_boss(BossJSONfileMap[name]);
	AddGOComponent(ReadJson);

	boss_name = ReadJson->GetBossName();
	index = ReadJson->GetIndex();
	is_boss_fight = ReadJson->IsBossFight();
	bpm = ReadJson->GetBPM();
	mp3_file_name = ReadJson->GetMp3();
	position = ReadJson->GetMovePosition();
	parttern = ReadJson->GetParttern();
	total_entry = ReadJson->GetTotalEntry();

}


void Boss::RunMusic()
{

}


void Boss::Draw(DrawLayer drawlayer)
{
	DrawCall draw_call = {
		GetGOComponent<Sprite>()->GetTexture(),                       // Texture to draw
		&GetMatrix(),                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader()
	};

	draw_call.settings.do_blending = true;
	GameObject::Draw(draw_call);
}

bool Boss::CanCollideWith(GameObjectTypes other_object) {
	switch (other_object) {
	case GameObjectTypes::Ship:
		return true;
	default:
		return false;
	}
}

void Boss::ResolveCollision(GameObject* other_object) {
	switch (other_object->Type())
	{
	case GameObjectTypes::Ship:

		break;
	}
}


