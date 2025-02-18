#pragma once

#include "GameObject.h"
#include "../Game/GameObjectTypes.h"

#include <filesystem>

class Icon : public GameObject
{
public:
	Icon(const std::string& alias, const std::filesystem::path& filename, vec2 position, float scale, bool drag);
	~Icon();
	GameObjectTypes Type() override { return GameObjectTypes::Icon; }
	std::string TypeName() override { return "Icon"; }

	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;

	const vec2 GetPosition() const { return position; }
	const float GetScale() const { return scale; }
	const std::string& GetAlias() const { return alias; }
	bool IsSelected() { return selected; }
	bool IsColliding() { return resolve_collision; }
	void SetSelected(bool select) { selected = select; }

private:
	std::string alias;
	vec2 position;
	float scale;
	bool selected = false;
	bool can_drag = false;
	bool resolve_collision = false;
};