#include "Boss.h"
#include "../Engine/Rapidjson.h"
#include "../Engine/AudioManager.h"




std::vector<std::string> BossJSONfileMap;


Boss::Boss(vec2 start_position, BossType type) 
	: GameObject(start_position)
{
	ReadBossJSON(type);
	AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
	SetVelocity({0,0});
	
}


void Boss::Update(double dt) {
	GameObject::Update(dt);
	if (Engine::GetGameStateManager().GetStateName() == "Mode1") {

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
	mp3 = ReadJson->GetMp3();
	move_position = ReadJson->GetMovePosition();
	parttern = ReadJson->GetParttern();
	total_entry = ReadJson->GetTotalEntry();

}

void Boss::Draw() 
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