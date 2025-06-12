#include "Player.h"
#include "../Engine/GameObjectManager.h"
#include "GameOption.h"

Player::Player(vec2 start_position) :
	GameObject(start_position)
{
	AddGOComponent(new Sprite("assets/images/character/character_sprite/Player.spt", this));
	current_state = &state_idle;
	current_state->Enter(this);
	SetScale({ 2.0f, 2.0f });
}

void Player::Update(double dt)
{
	if (!Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<GameOption>()->isOpened())
	{
		GameObject::Update(dt);

		if (current_state == &state_walking)
		{
			if (!soundPlaying)
			{
				Engine::GetAudioManager().PlayMusics("walk");
				soundPlaying = true;
			}
			else
			{
				if (!replay)
				{
					Engine::GetAudioManager().RestartPlayMusic("walk");
					replay = true;
				}
			}
		}
		else
		{
			Engine::GetAudioManager().StopPlayingMusic("walk");
			replay = false;
			soundPlaying = false;
		}
	}
	else
	{/*
		SetVelocity({ 0,0 });
		GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));*/
	}

}

void Player::Draw(DrawLayer drawlayer)
{
	GameObject::Draw(drawlayer);
}

void Player::Player_Idle::Enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Idle));
	player->SetVelocity({});
}

void Player::Player_Idle::Update(GameObject* object, double dt)
{
}

void Player::Player_Idle::CheckExit(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	if (Engine::GetInput().KeyDown(Input::Keys::A)) {
		player->change_state(&player->state_walking);
		player->SetScale({ 2.0f, 2.0f });
	}
	else if (Engine::GetInput().KeyDown(Input::Keys::D)) {
		player->change_state(&player->state_walking);
		player->SetScale({ -2.0f, 2.0f });
	}
}

void Player::Player_Walking::Enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Walking));
}

void Player::Player_Walking::Update(GameObject* object, double dt)
{
	Player* player = static_cast<Player*>(object);
	if (Engine::GetInput().KeyDown(Input::Keys::D)) {
		player->SetVelocity({ player->speed, 0 });
	}
	else if (Engine::GetInput().KeyDown(Input::Keys::A)) {
		player->SetVelocity({ -player->speed, 0 });
	}
}

void Player::Player_Walking::CheckExit(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	if (Engine::GetInput().KeyJustReleased(Input::Keys::D) || Engine::GetInput().KeyJustReleased(Input::Keys::A)) {
		player->change_state(&player->state_idle);
	}
}