#include "Dragging.h"

static Icon* currentDraggingIcon = nullptr; // 현재 드래그 중인 아이콘 (없으면 nullptr)

Dragging::Dragging(Icon& object) : object(object)
{
    icon_first_pos = object.GetPosition();
}

Dragging::~Dragging()
{
    if (currentDraggingIcon == &object) {
        currentDraggingIcon = nullptr;
    }
}

void Dragging::Update(double dt)
{
    if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT))
    {
        // 다른 아이콘이 이미 드래그 중이면 무시
        if (object.IsSelected() && (currentDraggingIcon == nullptr || currentDraggingIcon == &object))
        {
            currentDraggingIcon = &object; // 현재 드래그 중인 아이콘 지정
            object.SetPosition({
                Engine::GetInput().GetMousePosition().x - Engine::window_width / 2,
                Engine::GetInput().GetMousePosition().y - Engine::window_height / 2
                });
        }
    }
    else
    {
        // 마우스를 놓으면 원래 위치로 복귀
        if (currentDraggingIcon == &object)
        {
            object.SetPosition(icon_first_pos);
            object.SetSelected(false);
            currentDraggingIcon = nullptr; // 드래그 해제
        }
    }
}