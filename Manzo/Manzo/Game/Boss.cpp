#include "../Engine/Rapidjson.h"
#include "../Engine/AudioManager.h"
#include "Boss.h"


std::vector<CS230::GameObject::State*> stateMap;
std::vector<std::string> BossJSONfileMap;

Boss::Boss(vec2 start_position, BossType type) 
	: GameObject(start_position)
{
	ReadBossJSON(type);
	InitializeStates();
	AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
	SetVelocity({0,0});

	// cutscean
	
	////////
	current_state = &state_cutscene;
	current_state->Enter(this);


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

	if (Engine::GetInput().KeyDown(CS230::Input::Keys::R)&& boss->beat->GetBeat()) {
		Mix_Music* e_music = Engine::GetAudioManager().LoadMusic(boss->mp3_file_name, "E_Music");
		if (e_music) {
			Engine::GetAudioManager().PlayMusic(e_music, -1);
		}
		boss->beat->SetBPM(boss->bpm);
		boss->change_state(&boss->entry1);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry1::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry1::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
	int targetEntryNum = 1; 
	if (targetEntryNum - 1 < boss->parttern.size()) {
		const auto& entryVec = boss->parttern[targetEntryNum - 1]; 
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == boss->beat->GetDelayCount()) {
				boss->current_position = entryData.position;
				std::cout << "Position updated to: (" << entryData.position.x << ", " << entryData.position.y << ")" << std::endl;
			}
		}
	}
}
void Boss::Entry1::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
/// /////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry2::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry2::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
	int targetEntryNum = 2;
	if (targetEntryNum - 1 < boss->parttern.size()) {
		const auto& entryVec = boss->parttern[targetEntryNum - 1];
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == boss->beat->GetDelayCount()) {
				boss->current_position = entryData.position;
				std::cout << "Position updated to: (" << entryData.position.x << ", " << entryData.position.y << ")" << std::endl;
			}
		}
	}
}
void Boss::Entry2::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
////////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry3::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry3::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
	int targetEntryNum = 3;
	if (targetEntryNum - 1 < boss->parttern.size()) {
		const auto& entryVec = boss->parttern[targetEntryNum - 1];
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == boss->beat->GetDelayCount()) {
				boss->current_position = entryData.position;
				std::cout << "Position updated to: (" << entryData.position.x << ", " << entryData.position.y << ")" << std::endl;
			}
		}
	}
}
void Boss::Entry3::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
////////////////////////////////////////////////////////////////////////////////////////////
void Boss::Entry4::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry4::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
	int targetEntryNum = 4;
	if (targetEntryNum - 1 < boss->parttern.size()) {
		const auto& entryVec = boss->parttern[targetEntryNum - 1];
		for (const auto& entryData : entryVec) {
			if (entryData.delay + 1 == boss->beat->GetDelayCount()) {
				boss->current_position = entryData.position;
				std::cout << "Position updated to: (" << entryData.position.x << ", " << entryData.position.y << ")" << std::endl;
			}
		}
	}
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
	GameObject::Update(dt);

	if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
		if (GameObject::current_state->GetName() != Boss::state_cutscene.GetName()) {
			int barCount = beat->GetBarCount();
			std::cout << barCount<< std::endl;
			if(barCount <14){
			if (barCount < total_entry.size() && total_entry[barCount] - 1 < stateMap.size()) {
				change_state(stateMap[total_entry[barCount] - 1]);
			}
			else  {
				
				std::cerr << "Invalid barCount or index out of range: " << barCount << std::endl;
			}
			}
			else {
				Destroy();
				Engine::GetAudioManager().StopMusic();
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
	lerp_factor += (float)dt * 0.5f;  
	lerp_factor = std::min(lerp_factor, 1.0f); 

	vec2 lerped_position = Lerp(GetPosition(), current_position, lerp_factor);

	SetVelocity((lerped_position - GetPosition()) / (float)dt);  

	if ((current_position - GetPosition()).Length() < 10.0f) {
		lerp_factor = 0.0f; 
	}
}



void Boss::LoadBossfile() {
	BossJSONfileMap.push_back("assets/jsons/boss_e.json");
	BossJSONfileMap.push_back("Please add file path here");
}

void Boss::ReadBossJSON(BossType type)
{
	CS230::JsonParser* ReadJson = new CS230::JsonParser(BossJSONfileMap[type]);
	AddGOComponent(ReadJson);

	boss_name = ReadJson->GetBossName();
	index = ReadJson->GetIndex();
	is_boss_fight = ReadJson->IsBossFight();
	bpm = ReadJson->GetBPM();
	mp3_file_name = ReadJson->GetMp3();
	move_position = ReadJson->GetMovePosition();
	parttern = ReadJson->GetParttern();
	total_entry = ReadJson->GetTotalEntry();

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