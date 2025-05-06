#pragma once

#include "../Engine/GameObject.h"

class PopUp : public GameObject
{
public:
	PopUp(vec2 position, const std::filesystem::path& sprite_file, bool moving = false);
	GameObjectTypes Type() override { return GameObjectTypes::UI; }
	std::string TypeName() override { return "Inventory UI"; }

	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;
	void SetPop(bool is_pop) { pop = is_pop; }
	bool GetPop() { return pop; }

private:
	vec2 startPos;
	vec2 endPos;

	bool pop = false;
	bool moving = false;

	float duration = 0.5f;
	float elapsed = 0.0f;
};