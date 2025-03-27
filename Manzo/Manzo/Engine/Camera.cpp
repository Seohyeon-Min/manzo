#include "Camera.h"
#include "WindowState.h"
#include <iostream>

Cam::Cam()
{
	caminfo.camera_view.SetFramebufferSize((int)Engine::window_width, (int)Engine::window_height);
}

void Cam::Update(double dt, const vec2& player_position, bool playerMove)
{
    target_position = player_position;
    caminfo.camera.Position.x += (target_position.x - caminfo.camera.Position.x) * lerpFactor;
    caminfo.camera.Position.y += (target_position.y - caminfo.camera.Position.y) * lerpFactor;

	world_to_cam = caminfo.camera.BuildWorldToCamera();

	// cam_to_ndc   <- get CalculateCameraToNDC from camera view
	cam_to_ndc = caminfo.camera_view.BuildCameraToNDC();

	// world_to_ndc <- cam_to_ndc * world_to_cam
	world_to_ndc = cam_to_ndc * world_to_cam;
}


void Cam::SetPosition(vec2 new_position)
{
	caminfo.camera.Position = new_position;
}

const vec2& Cam::GetPosition() const
{
	return caminfo.camera.Position;
}