/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  GameObjectTypes.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once

enum class GameObjectTypes {
    Ship,
    Reef,
    ReefBoundary,
    Fish,
    Net,
    BackgroundFish,
    Particle,
    Boss,
<<<<<<< Updated upstream
    Count
=======
    Count,
    Shop,
    Icon,
    BossBullet
>>>>>>> Stashed changes
};

bool IsPixelShaderApplicable(GameObjectTypes type);