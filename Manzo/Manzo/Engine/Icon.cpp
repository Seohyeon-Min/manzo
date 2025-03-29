#include "Icon.h"
#include "GameObjectManager.h"
#include "../Game/Dragging.h"
#include "../Game/Mouse.h"

Icon::Icon(const std::string& alias, const std::filesystem::path& filename, vec2 position, float scale, bool drag, bool change_pos) : GameObject(position), alias(alias), position(position), scale(scale), can_drag(drag), can_change_pos(change_pos)
{
	AddGOComponent(new Sprite(filename, this));
	SetScale({ scale,scale });

	texture = Engine::GetTextureManager().Load(filename);

	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(this);
	AddGOComponent(new Dragging(*this));
}

Icon::~Icon()
{
	Destroy();
}

void Icon::Update(double dt)
{
	GameObject::Update(dt);
}

void Icon::Draw(DrawLayer drawlayer)
{
	DrawCall draw_call = {
	texture,                       // Texture to draw
	&GetMatrix(),                          // Transformation matrix
	Engine::GetShaderManager().GetShader("icon"), // Shader to use
	};

	draw_call.settings.do_blending = true;
	draw_call.SetUniforms = [this](const GLShader* shader) {shader->SendUniform("uTex2d", 0); };
	draw_call.SetUniforms = [this](const GLShader* shader) { shader->SendUniform("textureSize", (float)Engine::GetGameStateManager().GetGSComponent<Sprite>()->GetFrameSize().x, (float)Engine::GetGameStateManager().GetGSComponent<Sprite>()->GetFrameSize().y); };
	draw_call.sorting_layer = DrawLayer::DrawUI;

	Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
}

bool Icon::CanCollideWith(GameObjectTypes other_object)
{
	switch (other_object)
	{
	case GameObjectTypes::Mouse:
		if (can_drag) return true;
		return false;
	case GameObjectTypes::Icon:
		return true;
	}

	return false;
}

void Icon::ResolveCollision(GameObject* other_object)
{
	switch (other_object->Type())
	{
	case GameObjectTypes::Mouse:
		selected = true;
		break;
	case GameObjectTypes::Icon:
		resolve_collision = true;
		break;
	}
}