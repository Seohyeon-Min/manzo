#include "Ship.h"
#include "BeatSystem.h"
#include "../Engine/Camera.h"

#include <iostream>

Ship::Ship(Math::vec2 start_position) :
	GameObject(start_position), moving(false), set_dest(false), ready_to_move(false), move(false)
{
	AddGOComponent(new CS230::Sprite("assets/images/ship.spt", this));
	beat = Engine::GetGameStateManager().GetGSComponent<Beat>();
	SetVelocity({ 0,0 });
}

void Ship::Update(double dt)
{
	GameObject::Update(dt);
	SetDest();
	Move(dt);
}


void Ship::Draw(Math::TransformationMatrix camera_matrix)
{
	GameObject::Draw(camera_matrix);
}

void Ship::SetDest()
{
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !set_dest && beat->GetIsOnBeat() && !move) {
		std::cout << "yes\n";
		destination = { (double)GetMouseX(), (double)GetMouseY() };
		destination.y *= -1;
		destination.y += Engine::GetWindow().GetSize().y;
		set_dest = true;
	}

	if (set_dest) {	// if clicked the destination
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

    Math::vec2 direction = { destination.x - (GetPosition().x - (double)Engine::GetGameStateManager().GetGSComponent<CS230::Camera>()->GetPosition().x),
							 destination.y - (GetPosition().y - (double)Engine::GetGameStateManager().GetGSComponent<CS230::Camera>()->GetPosition().y)};
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
