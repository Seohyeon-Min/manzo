#pragma once

#include "../Engine/GameObject.h"

class Inven : public GameObject
{
public:
	Inven(vec2 position);
	GameObjectTypes Type() override { return GameObjectTypes::UI; }
	std::string TypeName() override { return "Inventory UI"; }

	void Update(double dt) override;
	void Draw(DrawLayer drawlayer = DrawLayer::Draw) override;

	bool GetIsOpened() { return is_opened; }
	void SetIsOpened(bool open) { is_opened = open; }

private:
	bool is_opened = false;
	int page = 0;

	enum class Animations {
		Module,
		FishCollection,
		SpecialCollection
	};

	class State_Module : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Inventory - Module"; }
	};

	class State_FC : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Inventory - Fish Collection"; }
	};

	class State_SC : public State {
	public:
		virtual void Enter(GameObject* object) override;
		virtual void Update(GameObject* object, double dt) override;
		virtual void CheckExit(GameObject* object) override;
		std::string GetName() override { return "Inventory - Special Collection"; }
	};

	State_Module state_module;
	State_FC state_fc;
	State_SC state_sc;
};