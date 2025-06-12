#include "GameOption.h"

#include "../Engine/Icon.h"
#include "../Engine/IconManager.h"
#include "States.h"

GameOption::GameOption(vec2 pos) : GameObject(pos)
{
	AddGOComponent(new Sprite("assets/images/Option.spt", this));
	shader = Engine::GetShaderManager().GetShader("window");
}

void GameOption::Update(double dt)
{
	if (Engine::GetInput().KeyJustPressed(Input::Keys::Esc)) {
		SetOpen(!isOpened());
	}

	if (opened)
	{
		GameObject::Update(dt);
		Engine::GetIconManager().ShowIconByGroup("Option");

		Icon* icon = Engine::GetIconManager().GetCollidingIconWithMouse({ Engine::GetInput().GetMousePos().mouseCamSpaceX ,Engine::GetInput().GetMousePos().mouseCamSpaceY });
		bool clicked = Engine::GetInput().MouseButtonJustPressed(SDL_BUTTON_LEFT);

		if (icon != nullptr) {
			Engine::GetGameStateManager().SetFromOption(true);

			if (Engine::GetGameStateManager().GetStateName() == "Mode2")
			{
				if ((icon->GetId() == "cali") && clicked) {
					Engine::GetGameStateManager().ClearNextGameState();
					Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Tutorial));
				}
				else if ((icon->GetId() == "tutorial") && clicked)
				{
					Engine::GetGameStateManager().ClearNextGameState();
					Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mode3));
				}
			}
		}
	}
	else
	{
		Engine::GetIconManager().HideIconByGroup("Option");
	}
}

void GameOption::Draw(DrawLayer drawlayer)
{
	if (opened)
	{
		if (Engine::GetGameStateManager().GetStateName() == "Mode2")
			layer = DrawLayer::Draw;
		else
			layer = DrawLayer::DrawLast;

		DrawCall draw_call = {
				GetGOComponent<Sprite>()->GetTexture(),
				&GetMatrix(),
				shader
		};

		draw_call.settings.do_blending = false;
		draw_call.settings.is_camera_fixed = true;
		draw_call.sorting_layer = layer;

		Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));

	}
}
