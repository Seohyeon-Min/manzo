#include "Dragging.h"

static Icon* currentDraggingIcon = nullptr; // 현재 드래그 중인 아이콘 (없으면 nullptr)

Dragging::Dragging(Icon& object) : object(object)
{
    icon_first_pos = object.GetPosition();
}

Dragging::~Dragging()
{
    currentDraggingIcon = nullptr;
    delete currentDraggingIcon;
}

void Dragging::Update(double dt)
{
    if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT))
    {
        if (object.IsSelected() && (currentDraggingIcon == nullptr || currentDraggingIcon == &object))
        {
            object.SetPosition({
                Engine::GetInput().GetMousePosition().x - Engine::window_width / 2,
                Engine::GetInput().GetMousePosition().y - Engine::window_height / 2
                });
        }
    }
    else if (currentDraggingIcon == &object)
    {
        object.SetPosition(icon_first_pos);
        object.SetSelected(false);
        currentDraggingIcon = nullptr;
    }
}
