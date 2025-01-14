#include "UI.h"
#include "DrawSetting.h"

void FuelUI::SetFuelBarUniforms(const GLShader* shader) {
    shader->SendUniform("uFilled", normalized_fuel);
}

FuelUI::FuelUI(Ship* ship) : ship(ship), draw_call((GLTexture*)nullptr,nullptr,nullptr)
{
    background_texture = Engine::GetTextureManager().Load("assets/images/ui_full.png");
    foreground_texture = Engine::GetTextureManager().Load("assets/images/ui.png");
    background_texture->SetFiltering(GLTexture::Linear);
    foreground_texture->SetFiltering(GLTexture::Linear);
    position = ship->GetPosition();
    Engine::GetShaderManager().LoadShader("health_bar", "assets/shaders/default.vert", "assets/shaders/health_bar.frag");
}

void FuelUI::Update(double dt)
{
    position = { ship->GetPosition().x + x_padding, ship->GetPosition().y - y_padding };
    parallax_matrix = mat3::build_translation(position) * mat3::build_scale(scale);
    normalized_fuel = ship->GetFuel() / ship->GetMaxFuel(); 
}

void FuelUI::AddDrawCall()
{

    draw_call = {
        background_texture,                       // Texture to draw
        &parallax_matrix,                          // Transformation matrix
        Engine::GetShaderManager().GetShader("health_bar"), // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { this->SetFuelBarUniforms(shader); };
    draw_call.sorting_layer = DrawLayer::DrawUI;

    Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));

    draw_call = {
        foreground_texture,                       // Texture to draw
        &parallax_matrix,                          // Transformation matrix
        Engine::GetShaderManager().GetDefaultShader() // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { this->SetFuelBarUniforms(shader); };
    draw_call.sorting_layer = DrawLayer::DrawUI;

    Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
}