#include "Icon.h"

Icon::Icon(vec2 position, GLTexture* icon_texture) : GameObject(position)
{
	SetVelocity({ 0, 0 });
	SetRotation(0.0);
	SetScale({ 0.4f, 0.4f });
	texture = icon_texture;
	
}

void Icon::Update(double dt)
{
	GameObject::Update(dt);
	//this->UpdateGOComponents(dt);
}

void Icon::Draw()
{
	DrawSettings settings;
	settings.is_UI = true;
	settings.do_blending = true;

	matrix = mat3::build_translation({ GetPosition() }) * mat3::build_scale(GetScale()); //* mat3::build_rotation(3.141592f/2.0f);

	draw_call =
	{
		texture,                       // Texture to draw
		&matrix,                          // Transformation matrix
		Engine::GetShaderManager().GetDefaultShader(), // Shader to use
		nullptr,
		settings
	};

	Engine::GetRender().AddDrawCall(draw_call, DrawLayer::DrawFirst);
}
