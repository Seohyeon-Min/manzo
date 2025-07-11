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
    Rock,
    MovingRock,
    RockBoundary,
    RockPoint,
    Fish,
    Net,
    BackgroundFish,
    Particle,
    Boss,
    Dialog,
    Count,
    Shop,
    Mouse,
    UI,
    Player,
    Icon,
    Monster,
    Module,
    BossBullet,
    Box,
    JellyEnemy,
    ObstacleRock
};

bool IsPixelShaderApplicable(GameObjectTypes type);