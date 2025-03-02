#include "BossBullet.h"
#include "../Engine/Collision.h"
#include <cmath>
#include <stdlib.h>
#include "Ship.h"
#include <vector>
#include <cmath>

#define PI  3.14159265358979
#define DEG2RAD (PI/180.0f)

BossBullet::BossBullet(vec2 Boss_position, float lifetime)
    :GameObject(Boss_position), lifetime(lifetime), timeElapsed(0.0f)
{
    AddGOComponent(new Sprite("assets/images/bullet.spt", this));

    static bool seedInitialized = false;
    if (!seedInitialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seedInitialized = true;
    }
    Ship* ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    vec2 shipPos = ship->GetPosition();
    vec2 direction = { shipPos.x - Boss_position.x, shipPos.y - Boss_position.y };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;


    this->velocity = vec2(0.0f, 0.0f); 
    float angleOffset = (float)(60 * DEG2RAD);
    float cosA = cos(angleOffset);
    float sinA = sin(angleOffset);

    vec2 modifiedDirection = {
                direction.x * cosA - direction.y * sinA,
                direction.x * sinA + direction.y * cosA
    };

    velocity = { -modifiedDirection.x * 200, -modifiedDirection.y * 200 };
}

void BossBullet::Update(double dt) {
    GameObject::Update(dt);

    Move(dt);

    // 수명 초과 시 삭제
    if (lifetime <= 0.0f) {
        this->Destroy();
    }
    else {
        lifetime -= dt; // 수명 감소
    }
}

void BossBullet::Move(double dt) {
    // Ship의 현재 위치 확인
    Ship* ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    vec2 shipPos = ship->GetPosition();

    // BossBullet과 Ship 사이의 거리 계산
    vec2 toPlayer = shipPos - this->position;
    float distanceToPlayer = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

    // Ship을 향해 이동하도록 속도 조정
    this->timeElapsed += dt;
    float speed = (float)(distanceToPlayer / this->lifetime) * (float)(this->timeElapsed / this->lifetime);  // 기본 속도
    float timeToReachPlayer = distanceToPlayer / speed; // Ship에 도달하는 데 걸리는 시간

    this->velocity += (normalize(toPlayer) * speed * (float)dt);
    this->position += -(this->velocity * (float)dt);
    SetVelocity(position);
}

void BossBullet::Draw(DrawLayer drawlayer) {
    DrawSettings settings;
    settings.do_blending = true;

    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),
        &GetMatrix(),
        Engine::GetShaderManager().GetDefaultShader(),
        nullptr,
        settings
    };

    GameObject::Draw(draw_call);
}

bool BossBullet::CanCollideWith(GameObjectTypes other_object) {
    return other_object == GameObjectTypes::Ship;
}

void BossBullet::ResolveCollision(GameObject* other_object) {
    if (other_object->Type() == GameObjectTypes::Ship) {
        this->Destroy();
    }
}
