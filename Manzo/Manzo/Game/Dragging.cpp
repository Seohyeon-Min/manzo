#include "Dragging.h"

#include <iostream>

Dragging::Dragging(Icon& object) : object(object)
{
	icon_first_pos = object.GetPosition();
}

void Dragging::Update(double dt)
{
	if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT) && object.IsSelected()) //이면서 is colliding with icon이면 아이콘 위치가 마우스 위치 따라 바뀜 
	{
		object.SetPosition({ Engine::GetInput().GetMousePosition().x - Engine::window_width / 2, Engine::GetInput().GetMousePosition().y - Engine::window_height/2});
	}
	else
	{
		object.SetPosition(icon_first_pos); //first position
		object.SetSelected(false);
	}
}