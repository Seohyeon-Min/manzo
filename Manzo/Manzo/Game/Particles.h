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
    class MouseFollow : public CS230::Particle {
    public:
        MouseFollow() : Particle("assets/images/mouse_part.spt") { };
        std::string TypeName() override { return "Mouse Particle"; }
        static constexpr int MaxCount = 30;
        static constexpr double MaxLife = 0.5;
    };

    class Plankton : public CS230::Particle {
    public:
        Plankton() : Particle("assets/images/Plankton.spt") { scale = util::random(0.5f, 4.6f); };
        std::string TypeName() override { return "Plankton Particle"; }
        static constexpr int MaxCount = 80;
        static constexpr double MaxLife = 10.5;
        float scale;
    };

    class FuelBubble : public CS230::Particle {
    public:
        FuelBubble() : Particle("assets/images/Plankton.spt") { scale = util::random(0.5f, 4.6f); };
        std::string TypeName() override { return "FuelBubble Particle"; }
        static constexpr int MaxCount = 15;
        static constexpr double MaxLife = 0.8;
        float scale;
    };
}

