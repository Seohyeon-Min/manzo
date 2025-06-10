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
    AddGOComponent(new Sprite("assets/images/boss/test_jelly_1.spt", this));
    texture_vector.push_back(Engine::GetTextureManager().Load("assets/images/boss/test_jelly_2.png"));
    texture_vector.push_back(Engine::GetTextureManager().Load("assets/images/boss/test_jelly_3.png"));
    SetScale(vec2{ 1.f,1.f });
    static bool seedInitialized = false;
    if (!seedInitialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seedInitialized = true;
    }

    ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    vec2 shipPos = ship->GetPosition();

    vec2 direction = { 0, 0 };
    switch (jelly_type)
    {
    case JellyType::Up:
        direction = { 0, +1 };
        break;
    case JellyType::Left:
        direction = { -1, 0 };
        break;
    case JellyType::Right:
        direction = { +1, 0 };
        break;
    case JellyType::Down:
        direction = { 0, -1 };
        break;
    case JellyType::ToPlayer:
        direction = shipPos - start_position;
        break;
    default:
        break;
    }

    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction = direction / length;

    wave_forward_dir = direction;  

    velocity = { 0, 0 };
    speed = 30;
    position = start_position;
    static_bullet = start_position;
    timeElapsed = 0.0f;
    wavePhaseOffset = (float)static_cast<float>(rand()) / (float)RAND_MAX * 2.0f * (float) PI;
    waveFrequencyOffset = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f; // 0.5~1.0 배
    waveAmplitudeOffset = 0.8f + static_cast<float>(rand()) / RAND_MAX * 0.4f; // 0.8~1.2 배


    procedural_jelly.Initialize(3, 15, position);
}

void JellyEnemy::Update(double dt) {
    GameObject::Update(dt);
    Move(dt);

    if (!std::isnan(position.x) && !std::isnan(position.y)) {
        procedural_jelly.Update(this, 0.07f);
        test_matrix =
            mat3::build_translation(procedural_jelly.GetPositions(0, mat3::build_scale(1.f))) *
            mat3::build_rotation(procedural_jelly.GetRotation(0, this)) *
            mat3::build_scale(1.f);

        test_matrix_1 =
            mat3::build_translation(procedural_jelly.GetPositions(1, mat3::build_scale(1.f))) *
            mat3::build_rotation(procedural_jelly.GetRotation(1, this)) *
            mat3::build_scale(1.f);
    }

    if (lifetime <= -1.0f) {
        this->Destroy();
        return;
    }
    else {
        lifetime -= dt;
    }

    /*Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEbulletParticle>>()->Emit(
        1, GetPosition(), { 0,0 }, {}, 1.5);*/
}

void JellyEnemy::Move(double dt) {
    this->timeElapsed += dt;

    vec2 perp = { -wave_forward_dir.y, wave_forward_dir.x }; // 수직 방향
    float waveOffset = sinf((float)timeElapsed * waveFrequency * waveFrequencyOffset + wavePhaseOffset)
        * waveAmplitude * waveAmplitudeOffset;
    vec2 movement = wave_forward_dir * speed_for_staticTarget * (float)dt + perp * waveOffset * (float)dt;
    this->position += movement;
    this->SetVelocity(movement / (float)dt); 
}


void JellyEnemy::Draw(DrawLayer drawlayer) {
    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),
        &GetMatrix(),
        Engine::GetShaderManager().GetDefaultShader()
    };
    draw_call.settings.do_blending = true;

    DrawCall draw_call2 = {
        texture_vector[0],
        &test_matrix,
        Engine::GetShaderManager().GetDefaultShader()
    };

    DrawCall draw_call3 = {
    texture_vector[1],
    &test_matrix_1,
    Engine::GetShaderManager().GetDefaultShader()
    };



    Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call3));
    Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call2));
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
