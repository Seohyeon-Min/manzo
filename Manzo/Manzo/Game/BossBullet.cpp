#include "BossBullet.h"
#include "../Engine/Collision.h"
#include "../Engine/Procedual.h"
#include "Particles.h"
#include <cmath>
#include <stdlib.h>
#include "Ship.h"
#include <vector>
#include <cmath>
#include "GameOption.h"

#define PI  3.14159265358979
#define DEG2RAD (PI/180.0f)

int GetRandomValue(int min, int max) {
    return min + rand() % ((max - min) + 1);
}

BossBullet::BossBullet(vec2 Boss_position, float lifetime, BulletType bullet_type)
    : GameObject(Boss_position), lifetime(lifetime), timeElapsed(0.0f), bulletType(bullet_type)
{
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        spawn_time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }

    AddGOComponent(new Sprite("assets/images/bullet.spt", this));
    float Scalerandom = (float)GetRandomValue(1, 2) * 0.11f;
    SetScale(vec2(Scalerandom));

    static bool seedInitialized = false;
    if (!seedInitialized) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seedInitialized = true;
    }

    ship = Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Ship>();
    vec2 shipPos = ship->GetPosition();
    vec2 direction = { shipPos.x - Boss_position.x, shipPos.y - Boss_position.y };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= length;
    direction.y /= length;

    this->velocity = vec2(0.0f, 0.0f);
    float angleOffset = (float)(GetRandomValue(-10, 10) * DEG2RAD);
    float cosA = cos(angleOffset);
    float sinA = sin(angleOffset);

    vec2 modifiedDirection = {
                direction.x * cosA - direction.y * sinA,
                direction.x * sinA + direction.y * cosA
    };


    velocity = { -modifiedDirection.x * 300, -modifiedDirection.y * 300 };
    speed = 30;
    position = Boss_position;
    this->static_bullet = Boss_position;
    timeElapsed = 0.0f;


    if (bulletType == BulletType::StaticTarget || bulletType == BulletType::Accelerating || bulletType == BulletType::Wave) {
        if (ship) {
            this->targetPosition = ship->GetPosition();
            vec2 dir = normalize(this->targetPosition - this->position);
            float initial_speed = (bulletType == BulletType::Accelerating) ? 10.0f : (float)speed_for_staticTarget;
            this->velocity = dir * initial_speed;
        }
    }

    if (bulletType == BulletType::Wave && ship) {
        this->targetPosition = ship->GetPosition();
        this->wave_forward_dir = normalize(this->targetPosition - this->position);
    }
    std::vector<int> circleSizes ={ 30, 20, 10 ,10,10};
    //procedual.Initialize(circleSizes, position);

    particle_timer = new Timer(0.0);
    AddGOComponent(particle_timer);
    particle_timer->Set(particle_time);
}

vec2 RandomPointAround(const vec2& center, float maxRadius = 15.0f) {
    float u = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float v = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    float r = maxRadius * std::sqrt(u);
    float theta = v * 2.0f * 3.141592f;

    float dx = std::cos(theta) * r;
    float dy = std::sin(theta) * r;

    return { center.x + dx, center.y + dy };
}

void BossBullet::Update(double dt) {
    GameObject::Update(dt);
    Move(dt);   

    //if (!std::isnan(position.x) && !std::isnan(position.y)) {
    //    procedual.Update(this, 0.2f);
    //}
    vec2 pos = GetPosition();
    
    if (lifetime <= -1.0f) {
        this->Destroy();
        procedual.Clear();

        return;
    }
    else
    {
        SetVelocity({ 0,0 });
    }


    if (lifetime <= 0.5)
    if (particle_timer->Remaining() <= 0.003) {
        vec2 bullet_pos = RandomPointAround(pos);
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEbulletParticle>>()->Emit(1, bullet_pos, {0,0}, -GetVelocity() * 0.1f, 1.5);
        ++kirakiracnt;
        if (kirakiracnt >= kirakiramax) {
            Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEKiraKiraParticle>>()->Emit(1, bullet_pos, { 0,0 }, -GetVelocity() * 0.1f, 1.5);
            kirakiracnt = 0;
        }
        particle_timer->Reset();
    }

        //Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEParticle1>>()->Emit(1, pos, GetVelocity(), {}, 1.5);
    vec2 bullet_pos = RandomPointAround(pos);
    if (lifetime <= 0.2) {
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEParticle1>>()->Emit(4, bullet_pos, GetVelocity() * 0.97f, -GetVelocity() * 0.1f, 0);
    }
    if(lifetime<=0.3)
        Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEParticle3>>()->Emit(2, bullet_pos, GetVelocity(), {}, 0);
    if (lifetime <= 0.5)
    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEParticle2>>()->Emit(2, bullet_pos, GetVelocity() * 0.7f, -GetVelocity(), 0);
    //Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::bossEParticle3>>()->Emit(1, GetPosition(), { 0,0 }, {}, 1.5);
}


void BossBullet::Move(double dt) {
    this->timeElapsed += dt;

    switch (bulletType)
    {
    case BossBullet::BulletType::Homing: {
        if (ship != nullptr) {
            this->targetPosition = ship->GetPosition();
        }
        toPlayer = this->targetPosition - this->position;
        distanceToPlayer = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

        float bulletspeed = (float)speed + (float)(this->distanceToPlayer / this->lifetime) * (float)(this->timeElapsed / this->lifetime);
        if (bulletspeed > 10000) bulletspeed = 10000;

        if (distanceToPlayer > 10.f) {
            this->velocity += normalize(toPlayer) * 1.2f * bulletspeed * (float)dt;
        }
        else {
            this->velocity += normalize(toPlayer) * 1.2f * bulletspeed * (float)dt;
        }

        this->position += this->velocity * 0.8f * (float)dt;
        this->SetVelocity(this->velocity);
        break;
    }

    case BulletType::StaticTarget: {
        this->static_bullet += this->velocity * (float)(dt);
        this->position = this->static_bullet;
        this->SetVelocity(this->velocity);
        break;
    }

    case BossBullet::BulletType::Wave: {
        vec2 perp = { -wave_forward_dir.y, wave_forward_dir.x }; // ���� ����
        float forwardSpeed = 300.f;
        float waveOffset = sinf((float)timeElapsed * waveFrequency) * waveAmplitude;

        vec2 movement = wave_forward_dir * forwardSpeed * (float)dt + perp * waveOffset * (float)dt;
        this->position += movement;
        this->SetVelocity(movement / (float)dt); // �浹 ����
        break;
    }

    case BossBullet::BulletType::Accelerating: {
        this->velocity *= 1.05f;
        this->position += this->velocity * (float)dt;
        SetVelocity(this->velocity);
        break;
    }

    default:
        break;
    }
}


void BossBullet::SetUni(const GLShader* shader)
{
    float time = 0.f;
    if (Engine::GetAudioManager().IsAnyMusicPlaying()) {
        time = Engine::GetAudioManager().GetCurrentPlayingMusicTime();
    }
    shader->SendUniform("uTime", time);
    shader->SendUniform("uStartTime", spawn_time);
    shader->SendUniform("uFadeDuration", 0.83f);
    shader->SendUniform("uStartColor", 0.f, 0.f, 0.f);
}


void BossBullet::Draw(DrawLayer drawlayer) {
    DrawCall draw_call = {
        GetGOComponent<Sprite>()->GetTexture(),
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("fade_in")
    };
    //procedual.Draw(GetMatrix(), drawlayer);


    draw_call.settings.do_blending = 1;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetUni(shader); };
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