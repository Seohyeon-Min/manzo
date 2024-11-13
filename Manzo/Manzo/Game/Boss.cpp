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

void Boss::Entry1::Enter(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
}

void Boss::Entry1::Update(GameObject* object, double dt) {
	Boss* boss = static_cast<Boss*>(object);
	//std::cout << "Get Beat : " << boss->beat->GetBeat() << std::endl;
	//std::cout << "Delay Count : " << boss->beat->GetDelayCount() << std::endl;
	//std::cout << boss->beat->GetBarCount() << std::endl;
	if ((boss->index)-1 < boss->parttern.size()) {
		const auto& entryVec = boss->parttern[(boss->index)-1]; 
		for (const auto& entryData : entryVec) {
			if (entryData.delay+1 == boss->beat->GetDelayCount()) {
				//std::cout << "entrydedt delay : " << entryData.delay << std::endl;
				//std::cout << "boss delay : " << boss->beat->GetDelayCount() << std::endl;
				boss->current_position = entryData.position;
				std::cout << "Position updated to: (" << entryData.position.x << ", " << entryData.position.y << ")" << std::endl;
			}
		}

	}
}
void Boss::Entry1::CheckExit(GameObject* object) {
	Boss* boss = static_cast<Boss*>(object);
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