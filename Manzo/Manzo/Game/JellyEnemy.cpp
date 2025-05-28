#include "JellyEnemy.h"
#include "../Engine/Collision.h"
#include "Particles.h"
#include "../Engine/Procedual.h"
#include <cmath>
#include <stdlib.h>
#include "Ship.h"
#include <vector>
#include <cmath>

#define PI  3.14159265358979
#define DEG2RAD (PI/180.0f)


JellyEnemy::JellyEnemy(vec2 start_position, float hight, float lifetime, JellyType jelly_type)
    : GameObject(start_position), hight(hight), lifetime(lifetime), timeElapsed(0.0f), jelly_type(jelly_type)
{

    //ProceduralChain procedural;
    AddGOComponent(new Sprite("assets/images/bullet.spt", this));

    static bool seedInitialized = false;
    if (!seedInitialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seedInitialized = true;
    }

    ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    vec2 shipPos = ship->GetPosition();
    vec2 direction = { 0,0 };
    switch (jelly_type)
    {
    case JellyEnemy::JellyType::Up:
        direction = {0, + start_position.y };
        break;
    case JellyEnemy::JellyType::Left:
        direction = { - start_position.x,0};
        break;
    case JellyEnemy::JellyType::Right:
        direction = { + start_position.x,0 };
        break;
    case JellyEnemy::JellyType::Down:
        direction = { 0, -start_position.y };
        break;
    case JellyEnemy::JellyType::ToPlayer:
        direction = { shipPos.x - start_position.x, shipPos.y - start_position.y };
        break;
    default:
        break;
    }

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;

    this->velocity = vec2(0.0f, 0.0f);



    velocity = { 0,0 };
    speed = 30;
    position = start_position;
    this->static_bullet = start_position;
    timeElapsed = 0.0f;
    
    //procedural.Initialize(3, 10, start_position);
}

void JellyEnemy::Update(double dt) {
    GameObject::Update(dt);
    Move(dt);

    if (lifetime <= -1.0f) {
        this->Destroy();
        return;
    }
    else {
        lifetime -= dt;
    }

    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::BulletParticle>>()->Emit(1, GetPosition(), { 0,0 }, /*-GetVelocity() * 0.4f*/{}, 1.5);

}


void JellyEnemy::Move(double dt) {
    this->timeElapsed += dt;

    vec2 perp = { -wave_forward_dir.y, wave_forward_dir.x }; // 수직 방향
    float forwardSpeed = 300.f;
    float waveOffset = sinf((float)timeElapsed * waveFrequency) * waveAmplitude;

    vec2 movement = wave_forward_dir * forwardSpeed * (float)dt + perp * waveOffset * (float)dt;
    this->position += movement;
    this->SetVelocity(movement / (float)dt); // 충돌 계산용
}


void JellyEnemy::Draw(DrawLayer drawlayer) {
    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),
        &GetMatrix(),
        Engine::GetShaderManager().GetDefaultShader()
    };

    draw_call.settings.do_blending = 1;
    GameObject::Draw(draw_call);
}

bool JellyEnemy::CanCollideWith(GameObjectTypes other_object) {
    return other_object == GameObjectTypes::Ship;
}

void JellyEnemy::ResolveCollision(GameObject* other_object) {
    if (other_object->Type() == GameObjectTypes::Ship) {

        this->Destroy();
    }
}
