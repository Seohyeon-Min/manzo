#include "Fish.h"
#include <random>
#include <iostream>
#include "../Engine/GameObjectManager.h"

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

BackgroundFish::BackgroundFish() : GameObject({start_position})
{
	ivec2 windowSize = { Engine::window_width, Engine::window_height };
	start_position = { -640 ,((float)rand() / RAND_MAX) * 2.0f * windowSize.y - windowSize.y }; //outside of window
	//start_position = { -500,100 };
	SetPosition(start_position);
	SetVelocity({30,0});

	AddGOComponent(new CS230::Sprite("assets/images/BackgroundFish.spt", this));
}

void BackgroundFish::Update(double dt) {
	GameObject::Update(dt);

	// Boids parameters
	const float separationDistance = 50.0f;
	const float alignmentWeight = 3.0f;
	const float cohesionWeight = 0.5f;
	const float separationWeight = 0.2f;
	const float wanderWeight = 0.1f;
	const float wanderJitter = 0.5f;
	const float baseSpeed = 30.0f;
	const float followWeight = 0.5f;  // Weight for following front fish
	const float reefAvoidanceWeight = 3.0f;

	vec2 alignment(0.0f, 0.0f);
	vec2 cohesion(0.0f, 0.0f);
	vec2 separation(0.0f, 0.0f);
	vec2 wanderForce(0.0f, 0.0f);
	vec2 followFront(0.0f, 0.0f);  // Vector for following the front fish
	vec2 avoidReef(0.0f, 0.0f);

	int neighborCount = 0;
	float closestFrontDistance = std::numeric_limits<float>::max();  // Track closest front fish

	// Calculate wander force
	wanderForce.x += ((float(rand()) / RAND_MAX) - 0.5f) * wanderJitter;
	wanderForce.y += ((float(rand()) / RAND_MAX) - 0.5f) * wanderJitter;
	wanderForce = wanderForce.Normalize() * wanderWeight;

	// Iterate over all fish to calculate flocking behavior
	for (auto& other : backgroundFishes) {
		if (other == this) continue;  // Ignore self

		vec2 toNeighbor = other->GetPosition() - GetPosition();
		float distance = toNeighbor.Length();

		// Only consider nearby fish as "neighbors"
		if (distance < separationDistance * 3.0f) {
			alignment += other->GetVelocity();
			cohesion += other->GetPosition();

			vec2 sub = toNeighbor / distance;
			if (distance < separationDistance) {
				separation.x -= sub.x;  // Weighted by inverse distance
				separation.y -= sub.y;
			}

			// Check if the other fish is in front along the x-axis
			if (other->GetPosition().x > GetPosition().x && distance < closestFrontDistance) {
				closestFrontDistance = distance;
				followFront = toNeighbor.Normalize();  // Normalize and store direction to front fish
			}

			neighborCount++;
		}
	}

	auto rocks = Engine::GetGameStateManager().GetGSComponent<CS230::Map>()->GetRock();
	for (auto& rock : rocks) {
		vec2 toReef = rock.GetPosition() - GetPosition();
		float distanceToReef = toReef.Length();

		// 장애물이 너무 가까우면 회피 행동 시작
		if (distanceToReef < separationDistance * 2.0f) {
			vec2 avoidanceDirection = -toReef.Normalize();

			vec2 avoidanceForce = avoidanceDirection * separationWeight * reefAvoidanceWeight;

			vec2 newVelocity = GetVelocity() + avoidanceForce;

			SetVelocity(newVelocity * baseSpeed);
		}
	}


	if (neighborCount > 0) {
		alignment = (alignment / (float)neighborCount).Normalize() * alignmentWeight;
		cohesion = ((cohesion / (float)neighborCount) - GetPosition()).Normalize() * cohesionWeight;
		separation = separation.Normalize() * separationWeight;
	}

	// Apply followFront with specified weight if a front fish is found
	followFront *= followWeight;

	// Combine boid forces, wander force, and followFront force
	vec2 boidForce = alignment + cohesion + separation + wanderForce + followFront;

	// Calculate oscillation for swimming effect
	float oscillationAmplitude = 5.0f;
	float oscillationFrequency = 1.0f;
	vec2 phaseOffset = { GetPosition().x * 10.0f, GetPosition().y * 5.0f };

	// Apply sine wave to velocity for a "swimming" effect
	float oscillatoryX = oscillationAmplitude * sin(oscillationFrequency * (float)dt + phaseOffset.x);
	float oscillatoryY = oscillationAmplitude * sin(oscillationFrequency * (float)dt + phaseOffset.y);

	// Adjust velocity and position based on combined forces and oscillatory effect
	vec2 finalVelocity = (GetVelocity() + boidForce).Normalize() * baseSpeed;
	finalVelocity.x += oscillatoryX;
	finalVelocity.y += oscillatoryY;

	SetVelocity(finalVelocity);

	// Destroy fish if it moves outside of world bounds
	if (GetPosition().x > Engine::window_width * 0.5f + 50) {
		Destroy();
	}
}

void BackgroundFish::Draw()
{
	GameObject::Draw();
}
