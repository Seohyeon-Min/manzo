/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Mode2.h
Project:    Manzo
Author:     Won Kim
Created:    Sept 12, 2024
*/

#pragma once
#include "../Engine/GameState.h"
#include "..\Engine\GameObjectManager.h"

class Background;

class Title : public GameState {
public:
    Title();
    void Load() override;
    void Update(double dt) override;
    void FixedUpdate(double dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override {
        return "Title";
    }

private:
    Background* background;
    const float start_alpha = 0.6f;
    float alpha;
    bool playing = false;
    float add_val = 0.0026f;
};

class TitleText : public GameObject {
public:
    TitleText(vec2 start_position);
    GameObjectTypes Type() override { return GameObjectTypes::Player; }
    std::string TypeName() override { return "Cat"; }
    void Update(double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
private:
    void SetUniform(const GLShader* shader);

};
