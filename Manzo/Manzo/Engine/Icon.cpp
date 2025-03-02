#include "Icon.h"
#include "GameObjectManager.h"
#include "../Game/Dragging.h"
#include "../Game/Mouse.h"

Icon::Icon(const std::string& alias, const std::filesystem::path& filename, vec2 position, float scale, bool drag, bool change_pos) : GameObject(position), alias(alias), position(position), scale(scale), can_drag(drag), can_change_pos(change_pos)
{
	AddGOComponent(new Sprite(filename, this));
	SetScale({ scale,scale });

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
	GameObject::Draw();
}

bool Icon::CanCollideWith(GameObjectTypes other_object)
{
	switch (other_object)
	{
	case GameObjectTypes::Mouse:
		if (can_drag) return true;
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