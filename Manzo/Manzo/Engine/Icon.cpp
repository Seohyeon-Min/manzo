#include "Icon.h"
#include "GameObjectManager.h"
#include "../Game/Dragging.h"
#include "../Game/Mouse.h"

Icon::Icon(const std::string& alias, const std::filesystem::path& filename, vec2 position, float scale, bool drag) : GameObject(position), alias(alias), position(position), scale(scale), can_drag(drag)
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
		else return false;
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
		//움직이고 있는 아이콘의 물고기 갯수 감소
		//마우스가 클릭하고 있지 않은 애는 움직이면 안됨 -> 다른 아이콘이랑 충돌시에는 flag값이 true. 이 값이 true면 마우스에 이미 누가 있다는 거니깐 안 움직여야됨. 이거를 겟함수 써서 드래깅 조건에다가 추가해놓으면 될듯
		break;
	}
}