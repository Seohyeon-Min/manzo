#include "Fish.h"
#include "Effect.h"
#include "ScreenWrap.h"

#include "../Engine/GameObjectManager.h"
#include "../Engine/AABB.h"
#include "../Engine/Camera.h"
#include "../Engine/Engine.h"

#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cmath>
#include <algorithm>
#include <map>

// 초기화 되는지 확인하기
std::map<int, int> fishCaptureCount;

#ifndef PIover3
#define PIover3  (3.1415926535987932f / 3.0f)
#endif

#ifndef PIover6
#define PIover6  (3.1415926535987932f / 6.0f)
#endif

std::mt19937 dre_fishIndex(rd());
static std::vector<Fish::FishDex> fishBook;
int Fish::money = 0;
int fishCnt = 0;
static std::vector<float> weights;
static std::vector<int> moneys;

Fish::Fish(Fish* parent) : GameObject({ 0, 0 }) {
    map = Engine::GetGameStateManager().GetGSComponent<MapManager>()->GetMap(0);

    std::discrete_distribution<> fishIndex(weights.begin(), weights.end());

    int index = fishIndex(dre_fishIndex);

    if (parent == nullptr) {
        bool isLeft = true; // rand() % 2;

        start_position = map->Spawn();//map->MaskToWorld(4700,5900);//{ startX , startY };
        SetPosition(start_position);
        
        //SetVelocity({ 0,0 });
        
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
    case GameObjectTypes::Net:
    case GameObjectTypes::Ship:
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(new CaptureEffect(GetPosition()));
        if (!collided)
        {
            collided = true;
            fishCnt--;
            money += moneys[type - 1];
            fishCaptureCount[type - 1]++;
        }
        this->Destroy();
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

    //temp destroy range
    //if (GetPosition().x < 0.f || GetPosition().x > 3520.f)
    //{
    //    this->Destroy();
    //}

    if(!map->IsMaskTrue(GetPosition()))
    {
        int max_attempts = 100;
        float try_distance = 100.0f; // 이동 거리

        for (int i = 0; i < max_attempts; ++i) {
            float angle_deg = RandomFloat(0.0f, 360.0f);
            float angle_rad = glm::radians(angle_deg);
            vec2 dir = { cos(angle_rad), sin(angle_rad) };
            vec2 testPos = GetPosition() + dir * try_distance;

            if (map->IsMaskTrue(testPos)) {
                SetVelocity(dir * GetVelocity().Length());  
                break;
            }
        }

    }
    else
    {

        //std::cout << "In Black" << std::endl;
    }
}

void Fish::Draw() {
    GameObject::Draw(DrawLayer::DrawLast);
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

        std::getline(linestream, cell, ',');
        f.possibility = std::stof(cell);
        weights.push_back(f.possibility);

        std::getline(linestream, cell, ',');
        f.money = std::stoi(cell);
        moneys.push_back(f.money);

        fishBook.push_back(f);
    }
    file.close();
}