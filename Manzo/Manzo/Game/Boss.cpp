#include "../Engine/Rapidjson.h"
#include "../Engine/AudioManager.h"
#include "Boss.h"



std::vector<std::string> BossJSONfileMap;

Boss::Boss(vec2 start_position, BossType type) 
	: GameObject(start_position)
{
	ReadBossJSON(type);
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
	boss->beat->SetBPM(boss->bpm);
}
void Boss::State_CutScene::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::State_CutScene::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
	if (Engine::GetInput().KeyJustPressed(CS230::Input::Keys::R)) {
		Mix_Music* e_music = Engine::GetAudioManager().LoadMusic(boss->mp3_file_name, "E_Music");
		if (e_music) {
			Engine::GetAudioManager().PlayMusic(e_music, -1);
		}
		boss->change_state(&boss->entry1);
	}
}

void Boss::Entry1::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry1::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
}
void Boss::Entry1::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
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
	mp3_file_name = ReadJson->GetMp3();
	move_position = ReadJson->GetMovePosition();
	parttern = ReadJson->GetParttern();
	total_entry = ReadJson->GetTotalEntry();

}

void Boss::RunMusic()
{

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