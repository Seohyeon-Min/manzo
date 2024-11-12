#pragma once
#include "Component.h"
#include "vec2.h"
#include "../Game/Ship.h"
#include <memory>

class UI : public CS230::Component {
public:
    virtual void AddDrawCall() = 0;
};

class FuelUI : public UI {
public:
    FuelUI(Ship* ship);
    void Update([[maybe_unused]] double dt) override;
    void AddDrawCall() override;
private:
    void SetFuelBarUniforms(const GLShader* shader);
    Ship* ship;
    GLTexture* background_texture;
    GLTexture* foreground_texture;
    vec2 position;
    CS230::DrawCall draw_call;
    mat3 parallax_matrix;
    float scale = 2.0f;
    float y_padding = 24.f;
    float x_padding = 8.f;
    float normalized_fuel = 1.0f;
};