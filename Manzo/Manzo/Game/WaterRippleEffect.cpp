#include "WaterRippleEffect.h"

WaterRipple::WaterRipple():
	GameObject({0,-340})
{
    AddGOComponent(new Sprite("assets/images/ship.spt", this));
    water = Engine::GetTextureManager().Load("assets/images/background/water.png");
}

void WaterRipple::Update(double dt)
{
    GameObject::Update(dt);
}

void WaterRipple::Draw(DrawLayer drawlayer)
{
    DrawCall draw_call = {
        water,
        &GetMatrix(),
        Engine::GetShaderManager().GetShader("water_ripple") // Shader to use
    };

    draw_call.settings.do_blending = true;
    draw_call.SetUniforms = [this](const GLShader* shader) { SetUni(shader); };
    draw_call.sorting_layer = drawlayer;

    GameObject::Draw(draw_call);
}

void WaterRipple::SetUni(const GLShader* shader) {
    float time = Engine::GetAudioManager().GetCurrentMusicTime("Level1_bgm");
    shader->SendUniform("uTime", time);
}