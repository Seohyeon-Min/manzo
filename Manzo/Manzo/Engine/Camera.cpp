#include "Camera.h"
#include <iostream>
CS230::Cam::Cam()
{
	//this->player_zone = player_zone;
	caminfo.camera_view.SetFramebufferSize(Engine::window_width, Engine::window_height);
}

void CS230::Cam::Update(double dt, const vec2& player_position, bool playerMove)
{

    float lerpFactor = 0.03f; // (0.0 ~ 1.0)
    vec2 target_position = player_position;
    caminfo.camera.Position.x += (target_position.x - caminfo.camera.Position.x) * lerpFactor;
    caminfo.camera.Position.y += (target_position.y - caminfo.camera.Position.y) * lerpFactor;

	world_to_cam = caminfo.camera.BuildWorldToCamera();

	// cam_to_ndc   <- get CalculateCameraToNDC from camera view
	cam_to_ndc = caminfo.camera_view.BuildCameraToNDC();

	// world_to_ndc <- cam_to_ndc * world_to_cam
	world_to_ndc = cam_to_ndc * world_to_cam;


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