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
	SetDest();
	//if (move) {
	//	Move();
	//}
}


void Ship::Draw(Math::TransformationMatrix camera_matrix)
{
	GameObject::Draw(camera_matrix);
}

void Ship::SetDest()
{
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !set_dest && beat->GetIsOnBeat()) {
		std::cout << "yes\n";
		destination = { (double)GetMouseX(), (double)GetMouseY() };
		destination.y *= -1;
		destination.y += Engine::GetWindow().GetSize().y;
		set_dest = true;
	}

	if (set_dest) {
		if (!beat->GetIsOnBeat()); {
			ready_to_move = true;
		}
	}

	if (ready_to_move && beat->GetBeat()) {
		SetPosition({ GetPosition().x - 50, GetPosition().y });
		move = true;
		set_dest = false;
		ready_to_move = false;
	}
}

void Ship::Move()
{
	Math::vec2 distance;
	distance = { destination.x - (GetPosition().x),
				 destination.y - (GetPosition().y) };
	double angle = atan2(distance.y, distance.x);

	double time = 0.5;  // �̵��Ϸ��� �ð� (��)
	double speed = 5.0;
	double move_distance = speed * time;

	// x��, y�������� ��ȭ�� ���
	double delta_x = move_distance * cos(angle);  // x�� �̵���
	double delta_y = move_distance * sin(angle);  // y�� �̵���

	// ���� ��ġ ������Ʈ
	SetVelocity({ delta_x , delta_y });
}

bool Ship::CanCollideWith(GameObjectTypes)
{
	return false;
}

void Ship::ResolveCollision(GameObject* other_object)
{
}
