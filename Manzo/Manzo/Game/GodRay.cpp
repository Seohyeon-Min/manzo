#include "GodRay.h"
#include "mat3.h"
#include "../Engine/Engine.h"

GodRay::GodRay()
{
	texture = Engine::GetTextureManager().Load("assets/images/full_quad.png");
}

void GodRay::Draw()
{
    const mat3 mat;
    CS230::DrawCall draw_call = {
    texture,                       // Texture to draw
    &mat,                          // Transformation matrix
    Engine::GetShaderManager().GetDefaultShader() // Shader to use
    };

    Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawLast);
}
