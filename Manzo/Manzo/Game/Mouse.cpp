#include "mouse.h"
#include "..\Engine\Input.h"
#include "Particles.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Mouse::Mouse()
{
}

void Mouse::Update(double dt)
{
	vec2 mouse_pos = Engine::GetInput().GetMousePosition();

	FollowMouse(mouse_pos);
}

void Mouse::FollowMouse(const vec2& mouse_position)
{
	if (Engine::GetInput().IsMouseMoving()) {
		Engine::GetGameStateManager().GetGSComponent<CS230::ParticleManager<Particles::MouseFollow>>()->Emit(2, mouse_position, { 0, 0 }, { 0, 100 }, 0);
	}

}

void Mouse::ExplodeOnClick(const vec2& click_position)
{

}
