#include "Fish.h"
#include <random>
#include <iostream>
#include "../Engine/GameObjectManager.h"
#include "../Engine/AABB.h"
#include <queue>

std::mt19937 dre;
std::vector<Fish::FishDex> Fish::fishBook;

int Fish::money = 0;

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
		parentFish = parent; // 부모 객체 저장
	}

	AddGOComponent(new CS230::Sprite(fishBook[index].filePath, this));
}

bool Fish::CanCollideWith(GameObjectTypes other_object)
{
	switch (other_object) {
	case GameObjectTypes::Ship:
	case GameObjectTypes::Net:
	case GameObjectTypes::ReefBoundary:
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
		this->Destroy();
		money++;
		break;

	case GameObjectTypes::ReefBoundary:
		vec2 avoidanceVelocity = AvoidRock(GetPosition(), { Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->FindNearestRock(this).x -20, Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->FindNearestRock(this).y - 20 });
		SetVelocity(GetVelocity() + avoidanceVelocity);  // 기존 속도에 회피 벡터를 더함
		IsAvoided = true;
		break;
	}
}

void Fish::Update(double dt) {
	GameObject::Update(dt);

	// 부모를 따라다니는 동작
	if (parentFish != nullptr) {
		// 부모와의 오프셋을 유지하며 이동
		SetVelocity(parentFish->GetVelocity());
	}

	// Destroy fish if it goes outside the world
	if (GetPosition().x - GetGOComponent<CS230::Sprite>()->GetFrameSize().x / 2 > Engine::window_width * 0.5f) {
		Destroy();
		return;
	}

	// 회피 로직
	vec2 currentPosition = GetPosition();
	vec2 nearestRock = { Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->FindNearestRock(this).x - 20, Engine::GetGameStateManager().GetGSComponent<CS230::GameObjectManager>()->FindNearestRock(this).y - 20 };

	if (IsRockInfront(currentPosition, nearestRock)) {
		if (!IsAvoided) {
			// 바위를 피하는 벡터 계산 (y축으로만 회피)
			vec2 avoidanceVelocity = AvoidRock(currentPosition, nearestRock);
			SetVelocity(GetVelocity() + avoidanceVelocity);  // 기존 속도에 회피 벡터를 더함
			IsAvoided = true;

			// 속도 제한
			const float maxSpeed = 200.0f;
			if (GetVelocity().Length() > maxSpeed) {
				SetVelocity(GetVelocity().Normalize() * maxSpeed);
			}
		}
	}
	else if (coolTime > 0) {
		coolTime -= dt;
	}
	else {
		coolTime = 1.5f;
		SetVelocity(fishBook[type - 1].velocity);
		IsAvoided = false;
	}
}

void Fish::Draw()
{
	CS230::GameObject::Draw();
}

vec2 Fish::AvoidRock(vec2 thisPos, vec2 rockPos) {
	vec2 distanceVec = rockPos - thisPos;

	// 단순화된 회피 벡터 계산 (y축으로만 반응하도록 수정)
	vec2 avoidanceVec = { 0, -distanceVec.y };  // x축은 무시하고 y축 방향으로만 회피

	// 회피 벡터의 크기 설정
	const float avoidanceStrength = 200.0f;  // 반발력 크기를 높여서 회피 효과 강하게 설정
	return avoidanceVec.Normalize() * avoidanceStrength;
}

bool Fish::IsRockInfront(vec2 thisPos, vec2 rockPos) {
	vec2 distanceVec = rockPos - thisPos;
	float detectionRadius = 150.0f; // 바위 탐지 거리
	float detectionCosAngle = std::cos(30.0f * (3.14159265f / 180.0f)); // 코사인 각도 계산

	// 거리 조건
	if (distanceVec.LengthSquared() > detectionRadius * detectionRadius) {
		return false;
	}

	// 각도 조건
	vec2 forwardVec = GetVelocity().Normalize();
	vec2 toRockVec = distanceVec.Normalize();

	return dot(forwardVec, toRockVec) >= detectionCosAngle;
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
			f.velocity = { velocitySize, 0 };

			std::getline(linestream, cell, ',');
			f.filePath = cell;

			fishBook.push_back(f);
		}
		file.close();
	}
}
