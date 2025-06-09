#include "UI.h"
#include "DrawSetting.h"

void FuelUI::SetFuelBarUniforms(const GLShader* shader) {
    shader->SendUniform("uFilled", normalized_fuel);
}

FuelUI::FuelUI(Ship* ship) : GameObject({}), ship(ship)
{
    background_texture = Engine::GetTextureManager().Load("assets/images/ship/ui_full.png");
    foreground_texture = Engine::GetTextureManager().Load("assets/images/ship/ui.png");
    //background_texture->SetFiltering(GLTexture::Linear);
    //foreground_texture->SetFiltering(GLTexture::Linear);
    position = ship->GetPosition();
    SetScale({ scale ,scale });
}

void FuelUI::Update(double dt)
{
    position = { ship->GetPosition().x + x_padding, ship->GetPosition().y - y_padding };
    SetPosition(position);
    normalized_fuel = ship->GetFuel() / ship->GetMaxFuel(); 
}

void FuelUI::Draw(DrawLayer drawlayer)
{
    DrawCall draw_call = {
        background_texture,                       // Texture to draw
        &GetMatrix(),                          // Transformation matrix
        Engine::GetShaderManager().GetShader("health_bar"), // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { this->SetFuelBarUniforms(shader); };
    draw_call.sorting_layer = DrawLayer::DrawUI;

    Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));

    DrawCall draw_call2 = {
        foreground_texture,                         // Texture to draw
        & GetMatrix(),                              // Transformation matrix
        Engine::GetShaderManager().GetDefaultShader() // Shader to use
    };

    draw_call2.settings.do_blending = true;
    draw_call2.SetUniforms = [this](const GLShader* shader) { this->SetFuelBarUniforms(shader); };
    draw_call2.sorting_layer = DrawLayer::DrawUI;

    Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call2));
}

