#include "Icon.h"
#include "GameObjectManager.h"

Icon::Icon(const std::filesystem::path& filename, vec2 position, float scale) : GameObject(position)
{
	AddGOComponent(new Sprite(filename, this));
	SetScale({ scale,scale });

	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(this);
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
	return (other_object == GameObjectTypes::Mouse);
}

void Icon::ResolveCollision(GameObject* other_object)
{
	switch (other_object->Type())
	{
	case GameObjectTypes::Mouse:
		break;
	}
}
