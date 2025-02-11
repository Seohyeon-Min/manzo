#pragma once
#include "..\Engine\Component.h"
#include "..\Engine\ComponentManager.h"
#include "..\Engine\GameObject.h"
#include "..\Engine\GameObjectManager.h"


class Dragging : public Component
{
public:

							Dragging(GameObject* object, GLTexture* tex); // need object's pointer and object's texture pointer
	void					Update(double dt);
	void					RevertPosition(bool input) { back_init_position = input; };


private:

	GameObject*				object_ptr;
	GLTexture*				texture;

	vec2					initial_position;
	bool					is_dragging = false;
	bool					back_init_position = false; // true = if dragging is end, going initial position, false = hold on position that end of drag position.
};