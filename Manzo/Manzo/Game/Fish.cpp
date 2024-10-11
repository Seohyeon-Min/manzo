#include "Fish.h"
#include <random>

std::mt19937 dre;

Fish::Fish(Fish* parent) : CS230::GameObject({ 0, 0 }) {

	std::uniform_int_distribution<int> random_value(0, 99);
	std::uniform_int_distribution<int> size(0, 2);

	if (parent == nullptr) {
		SetVelocity(vec2{
			((float)(rand() % 5) + 1.f) * default_velocity * 2.f,
			((float)(rand() % 5) + 0.1f) * default_velocity
			});

		ivec2 windowSize = { Engine::window_width, Engine::window_height };
		start_position = { -640 ,((float)rand() / RAND_MAX) * 2.0f * windowSize.y - windowSize.y }; //outside of window
		SetPosition(start_position);

		if (random_value(dre) < 20)  //20%
		{
			type = FishType::Fish3;
		}
		else if (random_value(dre) < 60)  //40%
		{
			type = FishType::Fish1;
		}
		else  //40%
		{
			type = FishType::Fish2;
		}

	}
	else
	{
		type = parent->type;
		SetVelocity(parent->GetPosition());
		SetPosition(parent->GetPosition());
		SetRotation(parent->GetRotation());
	}

	int scale_type = size(dre);
	SetScale(vec2{ static_cast<float>(default_scales[scale_type]), static_cast<float>(default_scales[scale_type]) });


	if (this->type == FishType::Fish2)
	{
		SetVelocity({ GetVelocity().x * 3.f, GetVelocity().y * 3.f });
	}

	switch (type)
	{
	case FishType::Fish1:
		AddGOComponent(new CS230::Sprite("assets/images/Fish1.spt", this));
		break;

	case FishType::Fish2:
		AddGOComponent(new CS230::Sprite("assets/images/Fish2.spt", this));
		break;

	case FishType::Fish3:
		AddGOComponent(new CS230::Sprite("assets/images/Fish3.spt", this));
		break;
	}

}

bool Fish::CanCollideWith(GameObjectTypes other_object)
{
	switch (other_object) {
	case GameObjectTypes::Ship:
		return true;
		break;
	}
	return false;
}

void Fish::ResolveCollision(GameObject* other_object)
{
	switch (other_object->Type()) {
	case GameObjectTypes::Ship:
		this->Destroy();
		break;
	}
}

void Fish::Update(double dt) {
	GameObject::Update(dt);

	if (GetPosition().x - GetGOComponent<CS230::Sprite>()->GetFrameSize().x / 2 > Engine::window_width * 0.5f ||
		GetPosition().y + GetGOComponent<CS230::Sprite>()->GetFrameSize().y / 2 < -Engine::window_height * 0.5f ||
		GetPosition().y - GetGOComponent<CS230::Sprite>()->GetFrameSize().y / 2 > Engine::window_height * 0.5f)
	{
		Destroy();
	}

	//to limit fish's moving range (y axis)
	if (GetPosition().y >= start_position.y + swimming_range || GetPosition().y <= start_position.y - swimming_range)
	{
		SetVelocity({ GetVelocity().x, -GetVelocity().y });
	}
}

void Fish::Draw()
{
	CS230::GameObject::Draw();
}
