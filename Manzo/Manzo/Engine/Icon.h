#pragma once

#include "GameObject.h"
#include "../Game/GameObjectTypes.h"

#include <filesystem>

class Icon : public GameObject
{
public:
	Icon(const std::filesystem::path& filename, vec2 position, float scale);
	GameObjectTypes Type() override { return GameObjectTypes::Icon; }
	std::string TypeName() override { return "Icon"; }

	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

private:
};