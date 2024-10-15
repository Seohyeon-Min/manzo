#include "Camera.h"
#include <iostream>
CS230::Cam::Cam(Math::rect player_zone)
{
    this->player_zone = player_zone;
	caminfo.camera_view.SetFramebufferSize(Engine::window_width, Engine::window_height);
}

void CS230::Cam::Update(double dt, const vec2& player_position)
{
    if (player_position.x > player_zone.Right() + caminfo.camera.Position.x) {
        caminfo.camera.Position.x = player_position.x - player_zone.Right();
    }
    if (player_position.x < player_zone.Left() + caminfo.camera.Position.x) {
        caminfo.camera.Position.x = player_position.x - player_zone.Left();
    }

    if (player_position.y > player_zone.Top() + caminfo.camera.Position.y) {
        caminfo.camera.Position.y = player_position.y - player_zone.Top();
    }
    if (player_position.y < player_zone.Bottom() + caminfo.camera.Position.y) {
        caminfo.camera.Position.y = player_position.y - player_zone.Bottom();
    }

    if (caminfo.camera.Position.x < limit.Left()) {
        caminfo.camera.Position.x = limit.Left();
    }
    if (caminfo.camera.Position.x > limit.Right()) {
        caminfo.camera.Position.x = limit.Right();
    }
    if (caminfo.camera.Position.y < limit.Bottom()) {
        caminfo.camera.Position.y = limit.Bottom();
    }
    if (caminfo.camera.Position.y > limit.Top()) {
        caminfo.camera.Position.y = limit.Top();
    }

    mat3 world_to_cam = caminfo.camera.BuildWorldToCamera();

    vec2 bottom_left = vec2{ limit.Left(), limit.Bottom() };
    vec2 bottom_right = vec2{ limit.Right(), limit.Bottom() };
    vec2 top_left = vec2{ limit.Left(), limit.Top() };
    vec2 top_right = vec2{ limit.Right(), limit.Top() };

    //draw limit
    /*Engine::GetRender().AddDrawCall(top_left, top_right, { 255, 0, 0 });
    Engine::GetRender().AddDrawCall(bottom_right, top_right, { 255, 0, 0 });
    Engine::GetRender().AddDrawCall(bottom_right, bottom_left, { 255, 0, 0 });
    Engine::GetRender().AddDrawCall(top_left, bottom_left, { 255, 0, 0 });*/

    //cam position
    Engine::GetRender().AddDrawCall({ caminfo.camera.Position.x-100 ,caminfo.camera.Position.y-100 }, { caminfo.camera.Position.x-100 , caminfo.camera.Position.y+100}, {0, 255, 255});
    Engine::GetRender().AddDrawCall({ caminfo.camera.Position.x - 100 ,caminfo.camera.Position.y - 100 }, { caminfo.camera.Position.x + 100 , caminfo.camera.Position.y - 100 }, { 0, 255, 255 });
    Engine::GetRender().AddDrawCall({ caminfo.camera.Position.x + 100 ,caminfo.camera.Position.y - 100 }, { caminfo.camera.Position.x + 100 , caminfo.camera.Position.y + 100 }, { 0, 255, 255 });
    Engine::GetRender().AddDrawCall({ caminfo.camera.Position.x - 100 ,caminfo.camera.Position.y + 100 }, { caminfo.camera.Position.x + 100 , caminfo.camera.Position.y + 100 }, { 0, 255, 255 });
}


void CS230::Cam::SetPosition(vec2 new_position)
{
	caminfo.camera.Position = new_position;
}

const vec2& CS230::Cam::GetPosition() const
{
	return caminfo.camera.Position;
}

void CS230::Cam::SetLimit(Math::rect new_limit)
{
	limit = new_limit;
}
