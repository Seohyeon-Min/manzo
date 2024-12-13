#include "Fish.h"
#include "DashEffect.h"
#include "ScreenWrap.h"

#include "../Engine/GameObjectManager.h"
#include "../Engine/AABB.h"
#include "../Engine/Camera.h"

#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cmath>
#include <algorithm>

#ifndef PIover3
#define PIover3  (3.1415926535987932f / 3.0f)
#endif
#ifndef PIover6
#define PIover6  (3.1415926535987932f / 6.0f)
#endif

std::mt19937 dre;
std::vector<Fish::FishDex> Fish::fishBook;
int Fish::money = 0;
int fishCnt = 0;

Fish::Fish(Fish* parent) : GameObject({ 0, 0 }) {
    std::uniform_int_distribution<int> fishIndex(0, static_cast<int>(fishBook.size() - 1));
    int index = fishIndex(dre);

    if (parent == nullptr) {
        ivec2 windowSize = { Engine::window_width, Engine::window_height };
        bool isLeft = rand() % 2;

        float startX = isLeft ? -800.f : 800.f;

        float startY = static_cast<float>(rand() % 720) - 360.f;

        start_position = { startX , startY };
        SetPosition(start_position);

        SetVelocity(isLeft ? fishBook[index].velocity : -fishBook[index].velocity);
        SetFlipX(!isLeft);

        SetScale(fishBook[index].scale);
        type = fishBook[index].type;
    }
    else {
        index = 2;
        type = parent->type;
        parentFish = parent;
    }

    AddGOComponent(new Sprite(fishBook[index].filePath, this));
    fishCnt++;

    AddGOComponent(new ScreenWrap(*this));
}

bool Fish::CanCollideWith(GameObjectTypes other_object) {
    switch (other_object) {
    case GameObjectTypes::Ship:
    case GameObjectTypes::Net:
        return true;
    default:
        return false;
    }
}

void Fish::ResolveCollision(GameObject* other_object) {
    switch (other_object->Type()) {
    case GameObjectTypes::Ship:
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
        if (!collided)
        {
            collided = true;
            fishCnt--;
            money++;
        }
        this->Destroy();
        break;

    case GameObjectTypes::Net:
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
        if (!collided)
        {
            collided = true;
            fishCnt--;
            money++;
        }
        this->Destroy();
        break;

    case GameObjectTypes::RockBoundary:
        vec2 avoidanceVelocity = AvoidRock(GetPosition(),
            { Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(this).x - 20,
              Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(this).y - 20 });
        SetVelocity(GetVelocity() + avoidanceVelocity);
        IsAvoided = true;
        break;
    }
}

void Fish::Update(double dt) {
    GameObject::Update(dt);

    SetFlipX(GetVelocity().x <= 0);

    if (parentFish != nullptr) {
        if (parentFish->Destroyed()) {
            parentFish = nullptr;
        }
        else {
            SetVelocity(parentFish->GetVelocity());
            SetRotation(parentFish->GetRotation());
            return;
        }
    }

    vec2 currentPosition = GetPosition();
    vec2 nearestRock = { (GetVelocity().x <= 0 ? Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(this).x + 20 : Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(this).x - 20),
                          Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->FindNearestRock(this).y - 20 };

    float safeDistance = 155.0f;  // safety distance

    // if it's avoiding some rock
    if (AvoidanceActive) {
        SetVelocity((GetVelocity() + AvoidanceVelocity) * 0.5f);

        // clear avoid state
        if ((currentPosition - nearestRock).LengthSquared() > safeDistance * safeDistance) {
            AvoidanceActive = false;
            IsAvoided = false;
            coolTime = 1.5f;  // reset cooltime
        }
        return;  // if it's avoiding, continue
    }

    if (IsRockInfront(currentPosition, nearestRock)) {
        if (!IsAvoided) {
            AvoidanceVelocity = AvoidRock(currentPosition, nearestRock);  // avoidance vector
            SetVelocity((GetVelocity() + AvoidanceVelocity) * 0.5f);

            IsAvoided = true;
            AvoidanceActive = true;  // avoiding now

            const float maxSpeed = 100.0f;
            if (GetVelocity().Length() > maxSpeed) {
                SetVelocity(GetVelocity().Normalize() * maxSpeed);
            }

            if (GetVelocity().x > 0) SetRotation(GetVelocity().y > 0 ? PIover3 : -PIover3);
            else SetRotation(GetVelocity().y <= 0 ? PIover3 : -PIover3);
        }
    }
    else if (coolTime > 0) {
        coolTime -= dt;
    }
    else {
        coolTime = 1.5f;
        SetVelocity(start_position.x >= 0 ? -fishBook[type - 1].velocity : fishBook[type - 1].velocity);
        SetRotation(0);
        IsAvoided = false;
    }
}

bool Fish::IsRockInfront(vec2 thisPos, vec2 rockPos) {
    vec2 distanceVec = rockPos - thisPos;
    float detectionRadius = 150.0f;
    float detectionCosAngle = std::cos(30.0f * (3.14159265f / 180.0f));

    if (distanceVec.LengthSquared() > detectionRadius * detectionRadius) {
        return false;
    }

    vec2 forwardVec = GetVelocity().Normalize();
    vec2 toRockVec = distanceVec.Normalize();

    return dot(forwardVec, toRockVec) >= detectionCosAngle;
}

vec2 Fish::AvoidRock(vec2 thisPos, vec2 rockPos) {
    vec2 distanceVec = rockPos - thisPos;
    vec2 avoidanceVec = { 0, -distanceVec.y};

    const float avoidanceStrength = 180.0f;
    return avoidanceVec.Normalize() * avoidanceStrength;
}


void Fish::Draw() {
    GameObject::Draw();
}
void Fish::ReadFishCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line, cell;
    std::getline(file, line);

    while (std::getline(file, line)) {
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