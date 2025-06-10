/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Particle.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/


#include "..\Game\GameObjectTypes.h"
#include "Sprite.h"
#include "ComponentManager.h"
#include "Random.h"
#include "angles.h"
#include "vec2.h"
#include "engine.h"
#include "GameObjectManager.h"
#include "logger.h"


#pragma once


enum class MoveType {
    LINEAR,
    CURVE,
    RANDOM,
    TOTHEPLAYER
};

class Particle : public GameObject {
public:
    Particle(const std::filesystem::path& sprite_file);
    void Start(vec2 position, vec2 velocity, double max_life, vec2 scale = vec2{1.f,1.f});
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
    void SetAlpha(const GLShader* shader);
    void SetMove(MoveType movetype, float intensity);
    bool Alive() {
        return  life > 0;
    }
    GameObjectTypes Type() { return GameObjectTypes::Particle; }
    std::string TypeName() override { return "Particle"; }
private:
    double life;
    double max_life;
    MoveType move_type = MoveType::LINEAR;
    float intensity = 1.0f;
protected:
    GLShader* shader = nullptr;
    DrawLayer drawlayer = DrawLayer::Draw;
    float life_offset = 0.0f;
};

template<typename T>
class ParticleManager : public Component {
public:
    ParticleManager();
    ~ParticleManager();
    void Emit(int count, vec2 emitter_position, vec2 emitter_velocity, vec2 direction, double spread);
    void EmitRound(int count, vec2 emitter_position, float emitter_velocity, float speed_variation);
    void Spray();
private:
    std::vector<T*> particles;
    int index;
};

template<typename T>
inline ParticleManager<T>::ParticleManager()
{
    for (int i = 0; i < T::MaxCount; i++) {
        T* data = new T;
        particles.push_back(data);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(data);
    }

}

template<typename T>
inline ParticleManager<T>::~ParticleManager()
{
    particles.clear();
}

template<typename T>
inline void ParticleManager<T>::Emit(int count, vec2 emitter_position, vec2 emitter_velocity, vec2 direction, double spread)
{
    mat3 m;
    for (int i = 0; i < count; i++) {
        //if(particles[index]->Alive())
        //    Engine::GetLogger().LogEvent("Particle overwritten");
        double angle_variation = 0.0;
        if (particles[index]->Alive())
            return;
        if (spread != 0) 
            angle_variation = ((rand() % static_cast<int>(spread * 1024)) / 1024.0f) - spread / 2;
        vec2 random_magnitude = direction * (((rand() % 1024) / 2048.0f) + 0.5f);
        vec2 particle_velocity = m.build_rotation(float(angle_variation)) * random_magnitude + emitter_velocity;
        T* plankton = static_cast<T*>(particles[index]);
        float scale = plankton->scale; // ���� Plankton ��ü�� scale �� ���
        particles[index]->Start(emitter_position, particle_velocity, T::MaxLife, { scale , scale });

        index++;
        if (index < 0 || index >= particles.size())
            index = 0;
    }
}

template<typename T>
inline void ParticleManager<T>::EmitRound(int count, vec2 emitter_position, float speed, float speed_variation) {
    mat3 m;
    for (int i = 0; i < count; i++) {
        if (particles[index]->Alive())
            continue;

        // 360�� ���� ��� (0 ~ 2�� ����)
        double angle = ((rand() % 1024) / 1024.0) * 2.0 * util::PI<double>;

        // ���� ���� ���
        vec2 direction = { (float)std::cos(angle), (float)std::sin(angle) };

        // �ӵ��� ������ ���� �߰�
        float random_speed = speed + ((rand() % 1024) / 1024.0f * 2.0f - 1.0f) * speed_variation;

        // �ӵ� ����
        vec2 particle_velocity = direction * random_speed;

        // ��ƼŬ �ʱ�ȭ �� ���� ����
        T* plankton = static_cast<T*>(particles[index]);
        float scale = plankton->scale;
        float angle_radians = std::atan2(direction.y, direction.x);

        particles[index]->Start(emitter_position, particle_velocity, T::MaxLife, { scale, scale });
        particles[index]->SetRotation(angle_radians);

        // �ε��� ����
        index++;
        if (index < 0 || index >= particles.size())
            index = 0;
    }
}

template<typename T>
inline void ParticleManager<T>::Spray()
{
    if (particles[index]->Alive())
        return;
    float speed = 30.f;
    float angle = (float)util::to_radians(util::random(30,150));
    float x = util::random(-1500.f, 1500.f);
    float y = util::random(-1500.f, 1500.f);
    vec2 pos = { x,y };
    vec2 vel = { cos(angle) * speed, sin(angle) * speed };

    T* plankton = static_cast<T*>(particles[index]);
    float scale = plankton->scale; // ���� Plankton ��ü�� scale �� ���

    particles[index]->Start(pos, vel, T::MaxLife, { scale , scale });
    index++;
    if (index < 0 || index >= particles.size())
        index = 0;
}

