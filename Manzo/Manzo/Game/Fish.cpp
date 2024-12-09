#include "Fish.h"
#include <random>
#include <iostream>
#include "../Engine/GameObjectManager.h"
#include "DashEffect.h"

std::mt19937 dre;
std::vector<Fish::FishDex> Fish::fishBook;

int Fish::money = 500;


Fish::Fish(Fish* parent) : GameObject({ 0, 0 }) {

	std::uniform_int_distribution<int> fishIndex(0, static_cast<int>(fishBook.size() - 1));
	int index = fishIndex(dre);

	if (parent == nullptr) {
		ivec2 windowSize = { (int)Engine::window_width, (int)Engine::window_height };
		start_position = { -640 ,((float)rand() / RAND_MAX) * 2.0f * windowSize.y - windowSize.y }; //outside of window
		SetPosition(start_position);
		SetVelocity(fishBook[index].velocity);
		SetScale(fishBook[index].scale);
		type = fishBook[index].type;
	}
	else
	{
		index = 2;
		type = parent->type;
	}

	AddGOComponent(new Sprite(fishBook[index].filePath, this));
}

bool Fish::CanCollideWith(GameObjectTypes other_object)
{
	switch (other_object) {
	case GameObjectTypes::Ship:
	case GameObjectTypes::Net:
		return true;
		break;
	}
	return false;
}

void Fish::ResolveCollision(GameObject* other_object)
{
	switch (other_object->Type()) {
	case GameObjectTypes::Ship:
	case GameObjectTypes::Net:
		Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
		this->Destroy();
		money++;
		break;
	}
}

void Fish::Update(double dt) {

	GameObject::Update(dt);

	//destroy outside world
	if (GetPosition().x - GetGOComponent<Sprite>()->GetFrameSize().x / 2 > Engine::window_width * 0.5f)
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
	GameObject::Draw();
}

void Fish::ReadFishCSV(const std::string& filename)
{
	std::ifstream        file(filename);
	std::string          line, cell;

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	if (file.is_open())
	{
		std::getline(file, line);

		while (std::getline(file, line))
		{
			std::stringstream linestream(line);
			FishDex f;
			float scaleSize, velocitySize;

			std::getline(linestream, cell, ',');
			f.type = static_cast<FishType>(std::stoi(cell));

			std::getline(linestream, cell, ',');
			scaleSize = std::stof(cell);
			f.scale = { scaleSize, scaleSize };

			std::getline(linestream, cell, ',');
			velocitySize = std::stof(cell);
			f.velocity = { velocitySize, velocitySize };

			std::getline(linestream, cell, ',');
			f.filePath = cell;

			fishBook.push_back(f);
		}
		file.close();
	}
}
