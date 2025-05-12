#pragma once

#include "GameObject.h"
#include "../Game/GameObjectTypes.h"

#include <filesystem>

class Icon : public GameObject
{
public:
	Icon(const std::string& id, const std::string& group, const std::filesystem::path& filename,
		vec2 position, float scale, bool drag, bool change_pos, bool interactiveWithMouse,
		bool draw, bool moving);

	~Icon();

	std::string GetId() const { return id; }
	GameObjectTypes Type() override { return GameObjectTypes::Icon; }
	std::string TypeName() override { return "Icon"; }

	void Update(double dt) override;
	//void FixedUpdate(double fixed_dt);
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

	bool CanCollideWith(GameObjectTypes) override;
	void ResolveCollision([[maybe_unused]] GameObject* other_object) override;
	
	const vec2 GetPosition() const { return position; }
	const float GetScale() const { return scale; }
	const std::string& GetGroup() const { return group; }

	bool IsSelected() { return selected; }
	bool IsColliding() { return resolve_collision; }
	bool CanChangePosition() { return can_change_pos; }
	bool CanDrag() {	return can_drag;	}
	bool IsHide() { return hide; }
	void SetSelected(bool select) { selected = select; }

	void SetHide(bool set) 
	{ 
		hide = set; 
		draw = !hide;
	}

	vec2 position;

private:
	GLTexture* texture;
	std::string id;
	std::string group;
	float scale;
	bool selected = false;
	bool can_drag = false;
	bool resolve_collision = false;
	bool can_change_pos = false;
	bool interaction = false;
	bool draw = true;
	bool hide = true;
};