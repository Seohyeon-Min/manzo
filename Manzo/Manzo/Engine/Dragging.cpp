#include "Dragging.h"

#include <iostream>

Dragging::Dragging(GameObject& object) : object(object)
{
	icon_first_pos = object.GetPosition();
}

void Dragging::Update(double dt)
{
	if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT)) //�̸鼭 is colliding with icon�̸� ������ ��ġ�� ���콺 ��ġ ���� �ٲ� 
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
1. 	if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT) && object.IsCollidingWith({ Engine::GetInput().GetMousePosition().x - Engine::window_width / 2, Engine::GetInput().GetMousePosition().y - Engine::window_height / 2 })) //�̸鼭 is colliding with icon�̸� ������ ��ġ�� ���콺 ��ġ ���� �ٲ� 
	- �̷��� üũ�ϸ� �±� �ѵ� ������ �ʹ� ���Ƽ� �ӵ��� ������ �� �����
*/