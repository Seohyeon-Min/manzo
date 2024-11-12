#include "UI.h"
#include "DrawSetting.h"

void FuelUI::SetFuelBarUniforms(const GLShader* shader) {
    shader->SendUniform("uFilled", normalized_fuel);
}

FuelUI::FuelUI(Ship* ship) : ship(ship)
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
    normalized_fuel = ship->GetFuel() / 500.0f; // 0 ~ 1로 정규화
}

void FuelUI::AddDrawCall()
{
    DrawSettings settings;
    settings.do_blending = true;

    draw_call = {
        background_texture,                       // Texture to draw
        &parallax_matrix,                          // Transformation matrix
        Engine::GetShaderManager().GetShader("health_bar"), // Shader to use
        [this](const GLShader* shader) {
            this->SetFuelBarUniforms(shader);
        }, 
        settings
    };

    Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);

    draw_call = {
        foreground_texture,                       // Texture to draw
        &parallax_matrix,                          // Transformation matrix
        Engine::GetShaderManager().GetDefaultShader(), // Shader to use
        [this](const GLShader* shader) {
            this->SetFuelBarUniforms(shader);
        },
        settings
    };

    Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);
}