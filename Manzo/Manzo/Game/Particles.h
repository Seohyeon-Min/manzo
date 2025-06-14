/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Particles.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#include "..\Engine\GameObjectManager.h"
#include "..\Engine\Particle.h"


#pragma once
namespace Particles {
    class MouseFollow : public Particle {
    public:
        MouseFollow() : Particle("assets/images/particle/mouse_part.spt") {  };
        std::string TypeName() override { return "Mouse Particle"; }
        static constexpr int MaxCount = 30;
        static constexpr double MaxLife = 0.5;
        float scale = 1.f;
    };

    class Plankton : public Particle {
    public:
        Plankton() : Particle("assets/images/particle/Plankton.spt") { scale = util::random(0.5f, 4.6f); };
        std::string TypeName() override { return "Plankton Particle"; }
        static constexpr int MaxCount = 40;
        static constexpr double MaxLife = 10.5;
        float scale;
    };

    class FuelBubble : public Particle {
    public:
        FuelBubble() : Particle("assets/images/particle/Plankton.spt") 
        {
            scale = util::random(0.5f, 1.f); shader = Engine::GetShaderManager().GetShader("change_alpha");
        };
        std::string TypeName() override { return "FuelBubble Particle"; }
        static constexpr int MaxCount = 7;
        static constexpr double MaxLife = 0.38;
        float scale;
    };

    class BubblePop : public Particle {
    public:
        BubblePop() : Particle("assets/images/particle/Plankton.spt") 
        { 
            scale = util::random(1.f, 2.2f); shader = Engine::GetShaderManager().GetShader("change_alpha");
        };
        std::string TypeName() override { return "BubblePop Particle"; }
        static constexpr int MaxCount = 8;
        static constexpr double MaxLife = 0.3;
        float scale;
    };

    class HitPraticle : public Particle {
    public:
        HitPraticle() : Particle("assets/images/particle/Smoke.spt")
        {
            scale = util::random(0.26f, 0.36f); shader = Engine::GetShaderManager().GetShader("change_alpha");
            drawlayer = DrawLayer::DrawPlayerTop;
        };
        std::string TypeName() override { return "HitEffect Particle"; }
        static constexpr int MaxCount = 20;
        static constexpr double MaxLife = 0.027;
        float scale;
    };

    class HitPraticle2 : public Particle {
    public:
        HitPraticle2() : Particle("assets/images/particle/Particle_black.spt")
        {
            scale = util::random(0.2f, 0.5f); shader = Engine::GetShaderManager().GetShader("change_alpha");
            drawlayer = DrawLayer::DrawLast;
        };
        std::string TypeName() override { return "HitPraticle2 Particle"; }
        static constexpr int MaxCount = 10;
        static constexpr double MaxLife = 0.21;
        float scale;
    };

    class CaptureEffect : public Particle {
    public:
        CaptureEffect() : Particle("assets/images/particle/Capture_particle.spt")
        {
            scale = util::random(.15f, .25f); shader = Engine::GetShaderManager().GetShader("change_alpha");
            drawlayer = DrawLayer::DrawPlayerTop;
        };
        std::string TypeName() override { return "CaptureEffect Particle"; }
        static constexpr int MaxCount = 71;
        static constexpr double MaxLife = 0.25;
        float scale;
    };


    class bossEbulletParticle : public Particle {
    public:
        bossEbulletParticle() : Particle("assets/images/particle/Plankton.spt")
        {
            scale = util::random(0.0f, 1.8f); shader = Engine::GetShaderManager().GetShader("change_alpha");
            life_offset = util::random(0.0f, 0.95f);
            drawlayer = DrawLayer::Draw;
        };
        std::string TypeName() override { return "CaptureEffect Particle"; }
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 1.05;
        float scale;
    };

    class bossEKiraKiraParticle : public Particle {
    public:
        bossEKiraKiraParticle() : Particle("assets/images/particle/kirakira_particle.spt")
        {
            scale = util::random(0.2f, 0.59f); shader = Engine::GetShaderManager().GetShader("emissive");
            life_offset = util::random(0.0f, 0.70f);
            drawlayer = DrawLayer::DrawLast;
        };
        std::string TypeName() override { return "CaptureEffect Particle"; }
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 0.80;
        float scale;
    };

    class bossEParticle1 : public Particle {
    public:
        bossEParticle1() : Particle("assets/images/particle/E_bullet_1.spt")
        {
            scale = util::random(0.16f, 0.20f); shader = Engine::GetShaderManager().GetShader("emissive");
            life_offset = util::random(0.0f, 0.12f);
            drawlayer = DrawLayer::DrawLast;
        };
        std::string TypeName() override { return "CaptureEffect Particle"; }
        static constexpr int MaxCount =50; 
        static constexpr double MaxLife = 0.18;
        float scale;
    };

    class bossEParticle2 : public Particle {
    public:
        bossEParticle2() : Particle("assets/images/particle/E_bullet_2.spt")
        {
            scale = util::random(0.0f, 0.5f); shader = Engine::GetShaderManager().GetShader("emissive");
            life_offset = util::random(0.0f, 0.1f);
            drawlayer = DrawLayer::Draw;
        };
        std::string TypeName() override { return "CaptureEffect Particle"; }
        static constexpr int MaxCount =50;
        static constexpr double MaxLife = 0.12;
        float scale;
    };

    class bossEParticle3 : public Particle {
    public:
        bossEParticle3() : Particle("assets/images/particle/E_bullet_3.spt")
        {
            scale = util::random(0.f, 0.8f);
            life_offset = util::random(0.0f, 0.3f);
            //life_offset = util::random(0.0f, 0.70f);
            drawlayer = DrawLayer::Draw;
        };
        std::string TypeName() override { return "CaptureEffect Particle"; }
        static constexpr int MaxCount = 50;
        static constexpr double MaxLife = 0.4;
        float scale;
    };

    class BossBlackCircleParticle : public Particle {
    public:
        BossBlackCircleParticle() : Particle("assets/images/effect/boss_black_circle.spt")
        {
            scale = util::random(0.1f, 0.4f); shader = Engine::GetShaderManager().GetShader("change_alpha");
            life_offset = util::random(0.0f, 0.8f);
            //life_offset = util::random(0.0f, 0.70f);
            drawlayer = DrawLayer::DrawFirst;
        };
        std::string TypeName() override { return "CaptureEffect Particle"; }
        static constexpr int MaxCount = 30;
        static constexpr double MaxLife = 1.6;
        float scale;
    };
}

