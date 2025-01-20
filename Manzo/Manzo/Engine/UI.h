#pragma once
#include "Component.h"
#include "vec2.h"
#include "../Game/Ship.h"
#include <memory>


class FuelUI : public GameObject {
public:
    FuelUI(Ship* ship);
    GameObjectTypes Type() override { return GameObjectTypes::UI; }
    std::string TypeName() override { return "Fuel_UI"; }
    void Update([[maybe_unused]] double dt) override;
    void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
private:
    FuelUI() :GameObject({}) {};
    void SetFuelBarUniforms(const GLShader* shader);
    Ship* ship;
    GLTexture* background_texture;
    GLTexture* foreground_texture;

    vec2 position;

    float scale = 0.34f;
    float y_padding = 24.f;
    float x_padding = 8.f;
    float normalized_fuel = 1.0f;
};