#include "Fish.h"
#include <random>
#include <iostream>

std::mt19937 dre;
std::vector<Fish::FishDex> Fish::fishBook;

Fish::Fish(Fish* parent) : CS230::GameObject({ 0, 0 }) {

	std::uniform_int_distribution<int> fishIndex(0, static_cast<int>(fishBook.size() - 1));
	int index = fishIndex(dre);

	if (parent == nullptr) {
		ivec2 windowSize = { Engine::window_width, Engine::window_height };
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

	AddGOComponent(new CS230::Sprite(fishBook[index].filePath, this));
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

	//destroy outside world
	if (GetPosition().x - GetGOComponent<CS230::Sprite>()->GetFrameSize().x / 2 > Engine::window_width * 0.5f)
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

BackgroundFish::BackgroundFish() : Fish(nullptr)
{
	ClearGOComponents();  //delete sprite added from fish class (change sprite from fish to background fish)

	SetScale({ 1.0,1.0 });
	SetVelocity({ 30,0 });

	AddGOComponent(new CS230::Sprite("assets/images/BackgroundFish.spt", this));
}

vec2 Lerp(const vec2& start, const vec2& end, float t)
{
	return start + (end - start) * t;
}

void BackgroundFish::Update(double dt) {
	if (leaderFish != nullptr) 
	{
		// Interpolate position for smoother movement
		vec2 targetPosition = { leaderFish->GetPosition().x - 30, leaderFish->GetPosition().y + 5 };
		vec2 currentPosition = GetPosition();

		// Use linear interpolation (lerp) for smooth following
		vec2 newPosition = Lerp(currentPosition, targetPosition, 0.1f); // Adjust the factor for smoothness
		SetPosition(newPosition);
	}

	Fish::Update(dt);  // Continue with the regular fish update logic
}


void BackgroundFish::Draw()
{
	Fish::Draw();
}

bool BackgroundFish::CanCollideWith(GameObjectTypes other_object)
{
	switch (other_object) {
	case GameObjectTypes::Reef:
		return true;
		break;
	}
	return false;
}

void BackgroundFish::ResolveCollision(GameObject* other_object)
{
	if (other_object->Type() == GameObjectTypes::Reef) {
		auto* collision_edge = this->GetGOComponent<CS230::RectCollision>();
		if (collision_edge != nullptr)
		{
			AvoidReef(collision_edge);
		}
	}
}

float GetDistanceToLine(const vec2& line_start, const vec2& line_end, const vec2& point) {
	// Calculate the perpendicular distance from a point to a line segment
	float line_length = (line_end - line_start).Length();
	if (line_length == 0) {
		return (point - line_start).Length(); // If the line length is 0, return the distance from start to point
	}
	float area = std::abs((line_end.x - line_start.x) * (line_start.y - point.y) - (line_start.x - point.x) * (line_end.y - line_start.y));
	return area / line_length;
}

void BackgroundFish::AvoidReef(CS230::RectCollision* collision_edge) {
	// Get the edges of the reef's collision area
	vec2 edge_1 = collision_edge->GetCollidingEdge().first;
	vec2 edge_2 = collision_edge->GetCollidingEdge().second;

	// Calculate the direction of the reef
	vec2 wall_dir = { edge_2.x - edge_1.x, edge_2.y - edge_1.y };
	vec2 wall_perpendicular = GetPerpendicular(wall_dir); // Perpendicular direction to the reef

	vec2 normal = wall_perpendicular.Normalize(); // Normal vector of the reef

	// Get the current position of the fish
	vec2 fish_position = GetPosition();
	vec2 fish_velocity = GetVelocity();

	// Calculate the closest distance to the reef (distance from fish to reef's line)
	float distance_to_reef = GetDistanceToLine(edge_1, edge_2, fish_position);

	// Define a threshold to start avoiding the reef (you can adjust this value)
	const float avoid_threshold = 20.0f;

	// If the fish is within the avoidance range, start avoiding
	if (distance_to_reef < avoid_threshold) {
		// Calculate the angle to steer the fish away from the reef
		float dot_product = fish_velocity.x * normal.x + fish_velocity.y * normal.y;
		if (dot_product > 0) {
			normal = normal * -1.0f;  // Reverse the normal if the fish is moving towards the reef
		}

		// Apply steering to adjust velocity
		vec2 steering = normal * 10.0f;  // Steering force (adjust multiplier for smoother or faster turns)
		vec2 new_velocity = fish_velocity + steering;

		// Update the fish's velocity to avoid the reef
		SetVelocity(new_velocity.Normalize() * fish_velocity.Length()); // Keep the same speed but change direction

		// Optionally, slightly adjust position to help the fish avoid the reef
		SetPosition(fish_position + normal * 10.0f); // Adjust position a bit to avoid immediate overlap
	}
}