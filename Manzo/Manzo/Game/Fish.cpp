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
	CS230::GameObject::Draw(DrawLayer::DrawFirst);
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
		if (collision_edge == nullptr) {
			// maybe an error?
		}
		HitWithRock(collision_edge);
	}
}

float BackgroundFish::PointToSegmentDistance(const vec2& point, const vec2& segmentStart, const vec2& segmentEnd) {
	vec2 segmentDir = segmentEnd - segmentStart;
	vec2 toPoint = point - segmentStart;

	float segmentLengthSquared = magnitude_squared(segmentDir);
	if (segmentLengthSquared == 0.0f) {
		return toPoint.Length();  // 선분의 길이가 0이면 점과 선분의 거리는 점과 점 사이의 거리
	}

	float projection = dot(toPoint, segmentDir) / segmentLengthSquared;
	projection = std::clamp(projection, 0.0f, 1.0f);  // [0, 1] 범위로 클램프

	vec2 closestPoint = segmentStart + projection * segmentDir;
	return (point - closestPoint).Length();
}

void BackgroundFish::HitWithRock(CS230::RectCollision* collision_edge)
{
	vec2 edge_1 = collision_edge->GetCollidingEdge().first;
	vec2 edge_2 = collision_edge->GetCollidingEdge().second;

	vec2 wall_dir = { edge_2.x - edge_1.x, edge_2.y - edge_1.y };
	vec2 wall_perpendicular = GetPerpendicular(wall_dir);
	vec2 normal = wall_perpendicular.Normalize();

	vec2 velocity = GetVelocity();

	float incoming_speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

	float dot_product_normal_velocity = velocity.x * normal.x + velocity.y * normal.y;
	if (dot_product_normal_velocity > 0) {
		normal = normal * -1.0f;
	}

	float dot_product = velocity.x * normal.x + velocity.y * normal.y;
	vec2 reflection = {
		velocity.x - 2 * dot_product * normal.x,
		velocity.y - 2 * dot_product * normal.y
	};

	SetPosition(GetPosition() + -GetVelocity() * 0.007f);
	direction = reflection.Normalize();
	SetVelocity(direction * incoming_speed * 0.75f);
	SetPosition(GetPosition() + normal * 0.5f);
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

	//rock이 있는 내부를 map tile triangle rasterization bounding box마냥 개작은 사각형들로 쪼개서 
	//내부는 1, 외부는 0 하고 0인 경로로만 이동하도록 하면 좋을 거 같은데
	//약간 지뢰찾기 느낌?
	
	auto rocks = Engine::GetGameStateManager().GetGSComponent<CS230::Map>()->GetRock();
	for (auto& rock : rocks) {
		auto polygon = rock.GetPolygon();  // 바위의 다각형 꼭짓점들 가져오기
		float minDistanceToRock = std::numeric_limits<float>::max();

		// 물고기 중심과 바위 다각형 간의 최소 거리 계산
		for (size_t i = 0; i < polygon.vertices.size(); ++i) {
			vec2 p1 = polygon.vertices[i];
			vec2 p2 = polygon.vertices[(i + 1) % polygon.vertices.size()];  // 인접한 두 꼭짓점

			// 물고기 중심에서 현재 두 꼭짓점 사이의 최소 거리 계산
			float dist = PointToSegmentDistance(GetPosition(), p1, p2);
			minDistanceToRock = std::min(minDistanceToRock, dist);
		}

		// 바위와의 최소 거리가 일정 범위 이내일 경우 피하는 로직
		if (minDistanceToRock < separationDistance * 2.0f) {
			vec2 toRock = rock.GetPosition() - GetPosition();
			vec2 avoidanceDirection = -toRock.Normalize();

			// 피하는 힘을 적용하여 속도 계산
			vec2 avoidanceForce = avoidanceDirection * separationWeight * reefAvoidanceWeight;

			// 현재 속도에 피하는 힘을 더하여 새로운 속도 계산
			vec2 newVelocity = GetVelocity() + avoidanceForce;

			// 새로운 속도로 물고기의 속도 설정
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
	GameObject::Draw(DrawLayer::DrawFirst);
}
