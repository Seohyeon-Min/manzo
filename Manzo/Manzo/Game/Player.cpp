#include "Player.h"

Player::Player(vec2 start_position) :
    GameObject(start_position)
{
    AddGOComponent(new Sprite("assets/images/ship.spt", this));
    current_state = &state_idle;
    current_state->Enter(this);
}

void Player::Update(double dt)
{
    GameObject::Update(dt);
}

void Player::Draw(DrawLayer drawlayer)
{
    GameObject::Draw(drawlayer);
}

void Player::Player_Idle::Enter(GameObject* object)
{
    Player* player = static_cast<Player*>(object);
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
        player->SetScale({ 1,1 });
    }
    else if (Engine::GetInput().KeyDown(Input::Keys::D)) {
        player->change_state(&player->state_walking);
        player->SetScale({ -1,1 });
    }
}

void Player::Player_Walking::Enter(GameObject* object)
{
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
