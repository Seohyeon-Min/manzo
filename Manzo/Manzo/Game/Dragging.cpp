#include "Dragging.h"
#include "..\Engine\Engine.h"
#include <typeinfo>

Dragging::Dragging(GameObject* object, GLTexture* tex)
{
	object_ptr = object;
	texture = tex;
	initial_position = object_ptr->GetPosition();
}

void Dragging::Update(double dt)
{
	vec2 drag_start_pos = { 0.0f, 0.0f }; // position of when start drag

	vec2 current_mouse_pos = { Engine::Instance().GetInput().GetMousePos().mouseCamSpaceX, Engine::Instance().GetInput().GetMousePos().mouseCamSpaceY }; // lastest mouse position

	if (!is_dragging) {
		if ((current_mouse_pos.x > object_ptr->GetPosition().x - (texture->GetWidth() / 4) &&
			current_mouse_pos.x < object_ptr->GetPosition().x + (texture->GetWidth() / 4)) &&
			(current_mouse_pos.y > object_ptr->GetPosition().y - (texture->GetHeight() / 4) &&
				current_mouse_pos.y < object_ptr->GetPosition().y + (texture->GetHeight() / 4))) // check mouse is on the object
		{
			if (Engine::Instance().GetInput().MouseButtonJustPressed(1)) { // Click Detection
#ifdef _DEBUG

				std::cout << "You clicked Object!" << std::endl;
#endif // _DEBUG
				// saving start position of drag
				is_dragging = true;
				drag_start_pos.x = current_mouse_pos.x;
				drag_start_pos.y = current_mouse_pos.y;
			}
		}
	}
	else {
			// while dragging
			if (Engine::Instance().GetInput().MouseButtonJustReleased(1)) { // if button released
#ifdef _DEBUG
				std::cout << "Released" << std::endl;
#endif
				if (back_init_position == true) // if need going initial position
				{
					object_ptr->SetPosition(initial_position); // go back!
				}

				is_dragging = false; // Release drag status

			}
			else {
				//follow mouse's position if keep dragging
				vec2 delta_pos = { current_mouse_pos.x - drag_start_pos.x, current_mouse_pos.y - drag_start_pos.y };
				object_ptr->SetPosition(delta_pos);
			}
		}
}
