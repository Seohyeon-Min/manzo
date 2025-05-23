#include "BossBullet.h"
#include "../Engine/Collision.h"
#include "Particles.h"
#include <cmath>
#include <stdlib.h>
#include "Ship.h"
#include <vector>
#include <cmath>

#define PI  3.14159265358979
#define DEG2RAD (PI/180.0f)

int GetRandomValue(int min, int max) {
    return min + rand() % ((max - min) + 1);
}

BossBullet::BossBullet(vec2 Boss_position, float lifetime)
    : GameObject(Boss_position), lifetime(lifetime), timeElapsed(0.0f)
{
    AddGOComponent(new Sprite("assets/images/bullet.spt", this));
    float Scalerandom = (float)GetRandomValue(1, 2);
    SetScale(vec2(Scalerandom));

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
    float angleOffset = (float)(GetRandomValue(-10,10) * DEG2RAD); 
    float cosA = cos(angleOffset);
    float sinA = sin(angleOffset);

    vec2 modifiedDirection = {
                direction.x * cosA - direction.y * sinA,
                direction.x * sinA + direction.y * cosA
    };

    velocity = { -modifiedDirection.x * 300, -modifiedDirection.y * 300 };
    speed = 30;
    position = Boss_position;
    timeElapsed = 0.0f;
}

void BossBullet::Update(double dt) {
    GameObject::Update(dt);
    Move(dt);

    if (lifetime <= -1.0f) {
        this->Destroy();
    }
    else {
        lifetime -= dt;
    }

    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::BulletParticle>>()->Emit(1, GetPosition(), { 0,0 }, /*-GetVelocity() * 0.4f*/{}, 1.5);

}


void BossBullet::Move(double dt) {
    Ship* ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    if (ship != nullptr) {
        this->targetPosition = ship->GetPosition();
    }
    toPlayer = this->targetPosition - this->position;
    distanceToPlayer = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
    this->timeElapsed += dt;

    float bulletspeed = (float)speed + (float)(this->distanceToPlayer / this->lifetime) * (float)(this->timeElapsed / this->lifetime);
    if (bulletspeed > 10000) {
        bulletspeed = 10000;
    }
    if (distanceToPlayer > 10.f) {
        this->velocity += (normalize(this->toPlayer) * bulletspeed * (float)dt);
        
    }
    else if (distanceToPlayer < 10.f) {
        this->velocity += (normalize(this->toPlayer) * bulletspeed * (float)dt);
        
    }
    this->position += (this->velocity) * 0.8f * (float)dt;
    
    SetVelocity(this->velocity);
}


void BossBullet::Draw(DrawLayer drawlayer) {
    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),
        &GetMatrix(),
        Engine::GetShaderManager().GetDefaultShader()
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
