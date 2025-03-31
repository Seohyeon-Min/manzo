#include "Module.h"

Module::Module() : GameObject({ 0,0 }) { }

Module::Module(vec2 start_position) : GameObject(start_position) { }

void Module::Update(double dt)
{
	GameObject::Update(dt);
}

void Module::Draw(DrawLayer drawlayer)
{
	if (Engine::GetGameStateManager().GetStateName() == "Mode1")
	{
		GameObject::Draw(drawlayer);
	}
}

// First Module
FirstModule::FirstModule(Ship* ship) : ship(ship)
{
	AddGOComponent(new Sprite("assets/images/module1.spt", this));

	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Module>()->SetFirstModule(true);
}

void FirstModule::Update(double dt)
{
	Module::Update(dt);
	SetPosition(ship->GetDashPos());
}

void FirstModule::Draw(DrawLayer drawlayer)
{
	//draw dot line
	LineDrawCall draw_call;
	vec2 start = { GetPosition().x, GetPosition().y };
	vec2 end = { ship->GetPosition().x, ship->GetPosition().y };

	vec2 direction = end - start;
	float distance = direction.Length();
	direction = direction.Normalize();

	float dash_length = 10.0f;
	float gap_length = 5.0f;

	float current_distance = 0.0f;
	bool draw = true;

	while (current_distance < distance) {
		vec2 segment_start = start + direction * current_distance;
		float next_distance = current_distance + dash_length;

		if (next_distance > distance) {
			next_distance = distance;
		}

		vec2 segment_end = start + direction * next_distance;

		if (draw) {
			LineDrawCall segment;
			segment.start = segment_start;
			segment.end = segment_end;
			segment.color = { 255, 255, 255 };
			segment.sorting_layer = DrawLayer::DrawUI;

			Engine::GetRender().AddDrawCall(std::make_unique<LineDrawCall>(segment));
		}

		draw = !draw;
		current_distance = next_distance + gap_length;
	}

	Module::Draw(drawlayer);
}

SecondModule::SecondModule(Ship* ship) : ship(ship)
{
	Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->GetGOComponent<Module>()->SetSecondModule(true);
}

void SecondModule::Update(double dt)
{
	if (Module::IsSecondSetted())
	{
		Module::Update(dt);
	}
}

void SecondModule::Draw(DrawLayer drawlayer)
{
	if (Module::IsSecondSetted())
	{
		Module::Draw(drawlayer);
	}
}
