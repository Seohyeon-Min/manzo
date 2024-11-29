#include "../Engine/Rapidjson.h"
#include "../Engine/AudioManager.h"
#include "Boss.h"
#include "Ship.h"


std::vector<CS230::GameObject::State*> stateMap;
std::vector<std::string> BossJSONfileMap;

Boss::Boss(vec2 start_position, BossName name, BossType type)
	: GameObject(start_position), bossType(type)
{
	ReadBossJSON(name);
	InitializeStates();
	AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
	current_position = start_position;


	// cutscean
	
	////////
	current_state = &state_cutscene;
	current_state->Enter(this);

}

void Boss::Movingtolocation_fun(int targetEntryNum, Boss* object) {
	if (targetEntryNum - 1 < object->parttern.size()) {
		const auto& entryVec = object->parttern[targetEntryNum - 1];
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == object->beat->GetDelayCount()) {
				object->current_position = entryData.position;
			}
		}
	}
}
void Boss::Chasingplayer_fun(int targetEntryNum, Boss* boss) {

	if (targetEntryNum - 1 < boss->parttern.size()) {
		const auto& entryVec = boss->parttern[targetEntryNum - 1];
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == boss->beat->GetDelayCount()) {
				Ship* ship = Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->GetGOComponent<Ship>();
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
					boss->current_position = bossPosition + (directionToPlayer * (boss->speed/4));
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

void Boss::Shooting_fun(int targetEntryNum, Boss* object) {

}
void Boss::MultiInstance_fun(int targetEntryNum, Boss* object) {

}

void Boss::Check_BossBehavior(int targetEntryNum, GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
	if (boss->bossType == Boss::BossType::MovingToLocation) 
	{
		Movingtolocation_fun(targetEntryNum, boss);
	}
	else if (boss->bossType == Boss::BossType::ChasingPlayer) 
	{
		Chasingplayer_fun(targetEntryNum, boss);
	}
	else if (boss->bossType == Boss::BossType::Shooting)
	{
		Shooting_fun(targetEntryNum, boss);
	}
	else if (boss->bossType == Boss::BossType::MultiInstance)
	{
		MultiInstance_fun(targetEntryNum, boss);
	}
	else
	{
		Engine::GetLogger().LogError("TYPE ERROR : There is no boss type like that... check one more time!");
	}
}

void Boss::State_CutScene::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
	Engine::GetAudioManager().StopMusic();
	boss->beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
	
}
void Boss::State_CutScene::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::State_CutScene::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
	if (Engine::GetInput().KeyDown(CS230::Input::Keys::R)) {
		Mix_Music* e_music = Engine::GetAudioManager().LoadMusic(boss->mp3_file_name, "E_Music");
		if (e_music) {
			Engine::GetAudioManager().PlayMusic(e_music, -1);
		}
		boss->beat->SetBPM(boss->bpm);
		std::cout <<"boss bpm:" << boss->bpm << std::endl;
		boss->change_state(&boss->entry1);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry1::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry1::Update(GameObject* object, double dt) {
	int targetEntryNum = 1; 
	Check_BossBehavior(targetEntryNum, object);
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

bool Boss::IsReefOutOfRange(vec2 point) {
	float distance = sqrtf((float)pow(point.x - GetPosition().x, 2) + (float)pow(point.y - GetPosition().y, 2));
	return distance > detectionRadius;
}

void Boss::Update(double dt) {
	Boss* boss = static_cast<Boss*>(this);
	CS230::GameObjectManager* gameobjectmanager = Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>();
	GameObject::Update(dt);
	vec2 nearestRockpoint = gameobjectmanager->FindNearestRock(boss);

	if (IsReefOutOfRange(currentTargetRock)) {
		currentTargetRock = gameobjectmanager->FindNearestRock(boss);
		
	}
	else if (currentTargetRock.x || currentTargetRock.y) {
		AvoidObstacle(200.0f, 0.1f, 300.0f, currentTargetRock);
		std::cout << "detected" << std::endl;
	}
	else {
		if (Engine::GetGameStateManager().GetStateName() == "Mode1") {

			if (GameObject::current_state->GetName() != Boss::state_cutscene.GetName()) {
				int barCount = beat->GetBarCount();
				//std::cout << total_entry.size() << std::endl;

				if (barCount < (int)total_entry.size()) {
					if (barCount < total_entry.size() && total_entry[barCount] - 1 < stateMap.size()) {
						change_state(stateMap[total_entry[barCount] - 1]);
					}
					else {

						std::cerr << "Invalid barCount or index out of range: " << barCount << std::endl;
					}
				}
				else if (barCount = (int)total_entry.size()) {
					Destroy();
					Engine::GetAudioManager().StopMusic();
				}
			}
		}
	}
	Move(dt);
}


vec2 Lerp(const vec2& start, const vec2& end, float t) {
	return start + t * (end - start);  
}

void Boss::Move(double dt) {
	vec2 direction = current_position - GetPosition();
	direction = direction.Normalize(); 

	vec2 force = direction * speed;

	static float lerp_factor = 0.0f;  
	lerp_factor += (float)dt * 0.1f;  
	lerp_factor = std::min(lerp_factor, 5.0f); 

	vec2 lerped_position = Lerp(GetPosition(), current_position, lerp_factor);

	SetVelocity((lerped_position - GetPosition()) / (float)dt);  

	if ((current_position - GetPosition()).Length() < 10.0f) {
		lerp_factor = 0.0f; 
	}
}



void Boss::LoadBossfile() {
	BossJSONfileMap.push_back("assets/jsons/boss_e.json");
	BossJSONfileMap.push_back("assets/jsons/boss_y.json");
	BossJSONfileMap.push_back("Please add file path here");
}

void Boss::ReadBossJSON(BossName name)
{
	CS230::JsonParser* ReadJson = new CS230::JsonParser(BossJSONfileMap[name]);
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

void Boss::AvoidObstacle(float avoidDistance, float steerSpeed, float speed, vec2 point) {
	vec2 bossPosition = GetPosition();
	vec2 reefPosition = point;


	vec2 currentVelocity = GetVelocity(); 

	float distanceFromObstacle = sqrtf((float)pow(bossPosition.x - reefPosition.x, 2) + (float)pow(bossPosition.y - reefPosition.y, 2));

	if (distanceFromObstacle < avoidDistance) {
		vec2 directionToObstacle = { reefPosition.x - bossPosition.x, reefPosition.y - bossPosition.y };
		float magnitude = sqrtf(directionToObstacle.x * directionToObstacle.x + directionToObstacle.y * directionToObstacle.y);

		if (magnitude > 0.0f) {
			directionToObstacle.x /= magnitude;
			directionToObstacle.y /= magnitude;
		}

		vec2 avoidDirection = { -directionToObstacle.x, -directionToObstacle.y };
		vec2 targetVelocity = { avoidDirection.x * speed, avoidDirection.y * speed };

		currentVelocity = Lerp(currentVelocity, targetVelocity, steerSpeed);
	}


	SetPosition({ bossPosition.x + currentVelocity.x, bossPosition.y + currentVelocity.y });
}

void Boss::RunMusic()
{

}


void Boss::Draw(DrawLayer drawlayer)
{
	CS230::GameObject::Draw();
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


