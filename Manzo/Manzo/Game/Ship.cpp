#include "Ship.h"
#include <iostream>

Ship::Ship(Math::vec2 start_position) :
	GameObject(start_position), moving(false), set_dest(false), ready_to_move(false), move(false)
{
	AddGOComponent(new CS230::Sprite("Assets/ship.spt", this));
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
{
	Math::vec2 currentPosition = GetPosition();
	float distanceMoved = (float)sqrt(pow(currentPosition.x - initialPosition.x, 2) + pow(currentPosition.y - initialPosition.y, 2));
	std::cout << currentSpeed << std::endl;
	if (distanceMoved >= 150.0f) {
		SetVelocity({ 0, 0 });
		currentSpeed = initialSpeed;
		move = false;
	}
	else {
		Math::vec2 direction = { (destination.x - GetPosition().x), (destination.y - GetPosition().y) };

		float magnitude = (float)sqrt(direction.x * direction.x + direction.y * direction.y);
		if (magnitude != 0) {
			direction.x /= magnitude;
			direction.y /= magnitude;
		}


		if (currentSpeed > 0) {
			currentSpeed -= (float)(deceleration); 
			if (currentSpeed < 0) currentSpeed = 0;
		}
		SetVelocity({ direction.x * currentSpeed, direction.y * currentSpeed });
	}

	
}

bool Ship::CanCollideWith(GameObjectTypes)
{
	return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
}
