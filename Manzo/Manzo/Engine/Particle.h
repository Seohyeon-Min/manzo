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

#include "vec2.h"
#include "engine.h"
#include "GameObjectManager.h"
#include "logger.h"


#pragma once
namespace CS230 {
    class Particle : public GameObject {
    public:
        Particle(const std::filesystem::path& sprite_file);
        void Start(vec2 position, vec2 velocity, double max_life);
        void Update(double dt) override;
        void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
        bool Alive() {
            return  life > 0;
        }
        GameObjectTypes Type() { return GameObjectTypes::Particle; }
        std::string TypeName() override { return "Particle"; }
    private:
        double life;
    };

    template<typename T>
    class ParticleManager : public Component {
    public:
        ParticleManager();
        ~ParticleManager();
        void Emit(int count, vec2 emitter_position, vec2 emitter_velocity, vec2 direction, double spread);
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

            if (spread != 0) 
                angle_variation = ((rand() % static_cast<int>(spread * 1024)) / 1024.0f) - spread / 2;
            vec2 random_magnitude = direction * (((rand() % 1024) / 2048.0f) + 0.5f);
            vec2 particle_velocity = m.build_rotation(float(angle_variation)) * random_magnitude + emitter_velocity;
            particles[index]->Start(emitter_position, particle_velocity, T::MaxLife);

            index++;
            if (index < 0 || index >= particles.size())
                index = 0;
        }
    }

}
