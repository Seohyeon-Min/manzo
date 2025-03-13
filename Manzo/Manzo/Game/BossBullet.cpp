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

    if (lifetime <= 0.0f) {
        this->Destroy();
    }
    else {
        lifetime -= dt; 
    }
}

void BossBullet::Move(double dt) {
    Ship* ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    vec2 shipPos = ship->GetPosition();

    vec2 toPlayer = shipPos - this->position;
    float distanceToPlayer = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

    this->timeElapsed += dt;
    float speed = (float)(distanceToPlayer / this->lifetime) * (float)(this->timeElapsed / this->lifetime);  
    float timeToReachPlayer = distanceToPlayer / speed; 

    this->velocity += (normalize(toPlayer) * speed * (float)dt);
    this->position += -(this->velocity * (float)dt);
    SetVelocity(position);
}

void BossBullet::Draw(DrawLayer drawlayer) {

    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),
        &GetMatrix()
    };

    draw_call.settings.do_blending = 1;
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
