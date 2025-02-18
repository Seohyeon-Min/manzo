#include "Dragging.h"

static Icon* currentDraggingIcon = nullptr; // ���� �巡�� ���� ������ (������ nullptr)

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
        // �ٸ� �������� �̹� �巡�� ���̸� ����
        if (object.IsSelected() && (currentDraggingIcon == nullptr || currentDraggingIcon == &object))
        {
            currentDraggingIcon = &object; // ���� �巡�� ���� ������ ����
            object.SetPosition({
                Engine::GetInput().GetMousePosition().x - Engine::window_width / 2,
                Engine::GetInput().GetMousePosition().y - Engine::window_height / 2
                });
        }
    }
    else
    {
        // ���콺�� ������ ���� ��ġ�� ����
        if (currentDraggingIcon == &object)
        {
            object.SetPosition(icon_first_pos);
            object.SetSelected(false);
            currentDraggingIcon = nullptr; // �巡�� ����
        }
    }
}