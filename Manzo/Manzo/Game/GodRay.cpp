#include "GodRay.h"
#include "mat3.h"
#include "BeatSystem.h"
#include "../Engine/Engine.h"


double counter = 0;

void GodRay::SetUniforms(const GLShader* shader) {
    double currentTime = Engine::GetAudioManager().GetCurrentMusicTime();
    counter += Engine::Instance().GetDt();
    shader->SendUniform("iResolution",Engine::window_width, Engine::window_height);
    shader->SendUniform("iTime", float(currentTime));
}

GodRay::GodRay()
{
    Engine::GetShaderManager().LoadShader("under_water_god_ray", "assets/shaders/default.vert", "assets/shaders/underwater_god_ray.frag");
	texture = Engine::GetTextureManager().Load("assets/images/full_quad.png");
}

void GodRay::Draw()
{
    DrawSettings settings;
    settings.do_blending = true;
    settings.is_UI = true;

    const mat3 mat;
    CS230::DrawCall draw_call = {
    texture,                       // Texture to draw
    &mat,                          // Transformation matrix
    Engine::GetShaderManager().GetShader("under_water_god_ray"), // Shader to use
    [this](const GLShader* shader) {                                                              
        this->SetUniforms(shader);
    },
    settings
    };

    Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawUI);
}

Pump::Pump()
{
    Engine::GetShaderManager().LoadShader("change_alpha", "assets/shaders/default.vert", "assets/shaders/change_alpha.frag");
    texture = Engine::GetTextureManager().Load("assets/images/full_quad.png");
}

void Pump::Draw()
{
    DrawSettings settings;
    settings.do_blending = true;
    //settings.modulate_color = true;
    settings.is_UI = true;

    const mat3 mat;
    CS230::DrawCall draw_call = {
    texture,                       // Texture to draw
    &mat,                          // Transformation matrix
    Engine::GetShaderManager().GetShader("change_alpha"), // Shader to use
    [this](const GLShader* shader) {
        this->SetUniforms(shader);
    },
    settings
    };

    Engine::GetRender().AddDrawCall(draw_call);
}

void Pump::Update(double dt)
{
    if (Engine::GetGameStateManager().GetGSComponent<Beat>()->GetBeat()) {
        alpha = 0.12f;
    }
    if(alpha>=0)
    alpha -= 0.002f;
}

void Pump::SetUniforms(const GLShader* shader)
{
    shader->SendUniform("uAlpha", alpha);
}
