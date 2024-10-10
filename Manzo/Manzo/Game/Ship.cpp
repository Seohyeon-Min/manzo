#include "Ship.h"
#include "BeatSystem.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include <iostream>

Ship::Ship(vec2 start_position) :
	GameObject(start_position), moving(false), set_dest(false), ready_to_move(false), move(false), floating(true), startPosition(start_position)
{
	AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
	beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
	SetVelocity({ 0,0 });
}

void Ship::Update(double dt)
{
	GameObject::Update(dt);
    Floating(dt);
	SetDest();
	if (move) {
        floating = false;
		Move(dt);
	}
}


void Ship::Draw()
{
	GameObject::Draw();
}

void Ship::SetDest()
{
    if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) && !set_dest && beat->GetIsOnBeat() && !move) {
        // Get mouse position relative to the center of the screen
        destination.x = Engine::GetInput().GetMousePosition().x;
        destination.y = Engine::GetInput().GetMousePosition().y;

        set_dest = true;
    }

    if (set_dest) { // if clicked the destination
        if (!beat->GetIsOnBeat()) { // wait for next beat
            ready_to_move = true;
        }
    }

    if (ready_to_move && beat->GetBeat()) { // move when its on next beat
        initialPosition = GetPosition();

        move = true;
        set_dest = false;
        ready_to_move = false;
    }
}

void Ship::Move(double dt)
{ // there is a bug that if destination is too short, ship doesn't move any more

    float distanceMoved = (float)sqrt(pow(GetPosition().x - initialPosition.x, 2) + pow(GetPosition().y - initialPosition.y, 2));


	vec2 direction = { destination.x - (GetPosition().x),
						 destination.y - (GetPosition().y) };
    //vec2 direction = { destination.x - (GetPosition().x - (float)Engine::GetGameStateManager().GetGSComponent<CS230::Camera>()->GetPosition().x),
				//			 destination.y - (GetPosition().y - (float)Engine::GetGameStateManager().GetGSComponent<CS230::Camera>()->GetPosition().y)};
    float magnitude = (float)sqrt(direction.x * direction.x + direction.y * direction.y);

    if (magnitude != 0) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }

    float totalDistanceToMove = 150.0f;

    if (distanceMoved >= totalDistanceToMove) { // stop
        SetVelocity({ 0, 0 });
        currentSpeed = initialSpeed;
        move = false;
    }
    else {
        if (currentSpeed > 0) {
            currentSpeed -= (float)(deceleration);
            if (currentSpeed < 0) currentSpeed = 0;
        }

        SetVelocity({ direction.x * currentSpeed, direction.y * currentSpeed }); //move if left
    }
}

bool Ship::CanCollideWith(GameObjectTypes other_object)
{
	switch (other_object) {
	case GameObjectTypes::Fish:
		return true;
		break;
	}

	return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
}

void Ship::Floating(double dt) {
    // ship is floating up and down...
    // we can change this into animation later.

    if (floating == true) {
        SetVelocity({ 0,-10 });
    }
    
}