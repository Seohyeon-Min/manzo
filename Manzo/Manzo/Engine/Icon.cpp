#include "Icon.h"
#include "GameObjectManager.h"
#include "../Game/Dragging.h"
#include "../Game/Mouse.h"
#include "../Game/PopUpMovement.h"

Icon::Icon(const std::string& id, const std::string& group, const std::filesystem::path& filename, vec2 position, float scale, bool drag, bool change_pos, bool interactiveWithMouse, bool draw, bool moving) 
	: GameObject(position), id(id), group(group), position(position), scale(scale), can_drag(drag), can_change_pos(change_pos), interaction(interactiveWithMouse), draw(draw)
{
	AddGOComponent(new Sprite(filename, this));
	SetScale({ scale,scale });

	AddGOComponent(new Dragging(*this));

	if (moving)
	{
		vec2 start = { position.x - 5000.f - GetGOComponent<Sprite>()->GetFrameSize().x, position.y };
		AddGOComponent(new PopUpMovement(this, start, position));
		SetPosition(start);
	}
}

Icon::~Icon()
{
	Destroy();
}

void Icon::Update(double dt)
{
	if (hide) return;
	GameObject::Update(dt); 
}


void Icon::Draw(DrawLayer drawlayer)
{
	if (draw)
	{
		DrawCall draw_call = {
		GetGOComponent<Sprite>()->GetTexture(),                       // Texture to draw
		&GetMatrix(),                          // Transformation matrix
		Engine::GetShaderManager().GetShader("icon"), // Shader to use
		};

		draw_call.settings.do_blending = true;
		draw_call.SetUniforms = [this](const GLShader* shader) {
			shader->SendUniform("uTex2d", 0);
			shader->SendUniform("textureSize",
				(float)GetGOComponent<Sprite>()->GetFrameSize().x,
				(float)GetGOComponent<Sprite>()->GetFrameSize().y);
			shader->SendUniform("canCollide", interaction);
			shader->SendUniform("isColliding", this->IsCollidingWith({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY }));
			};
		draw_call.sorting_layer = DrawLayer::Draw;

		Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
	}
}

bool Icon::CanCollideWith(GameObjectTypes other_object)
{
	if (hide) return false;
	if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) || (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT))) {}
	else return false;

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
	if (hide) return;
	if (Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT) || (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT))) {}
	else return;

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