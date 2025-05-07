#pragma once

#include "../Engine/MapManager.h"
#include "../Engine/GameObject.h"
#include "GameObjectTypes.h"


class Box : public GameObject
{
public:
	Box(vec2 position);
	GameObjectTypes Type() override { return GameObjectTypes::Box; }
	std::string TypeName() override { return "Box"; }
	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
	int GetMoney() { return money; }
	void ClearMoney() { money = 0; }
	void SetMoney(int count) { money = count; }

	void Update(double dt);
	void Draw();

private:
	static int money;
};
