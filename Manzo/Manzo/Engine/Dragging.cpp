#include "Dragging.h"

#include <iostream>

Dragging::Dragging(GameObject& object) : object(object)
{
	icon_first_pos = object.GetPosition();
}

void Dragging::Update(double dt)
{
	if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT)) //이면서 is colliding with icon이면 아이콘 위치가 마우스 위치 따라 바뀜 
	{
		//std::cout << "Is Clicked" << std::endl;
		object.SetPosition({ Engine::GetInput().GetMousePosition().x - Engine::window_width / 2, Engine::GetInput().GetMousePosition().y - Engine::window_height/2});
	}
	else
	{
		object.SetPosition(icon_first_pos); //first position
	}
}

/*
1. 	if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT) && object.IsCollidingWith({ Engine::GetInput().GetMousePosition().x - Engine::window_width / 2, Engine::GetInput().GetMousePosition().y - Engine::window_height / 2 })) //이면서 is colliding with icon이면 아이콘 위치가 마우스 위치 따라 바뀜 
	- 이렇게 체크하면 맞긴 한데 범위가 너무 좁아서 속도가 빠르면 못 따라옴
*/