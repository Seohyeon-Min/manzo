#include "Camera.h"
#include "WindowState.h"
#include "MapManager.h"
#include <iostream>

Cam::Cam()
{
	caminfo.camera_view.SetFramebufferSize((int)Engine::window_width, (int)Engine::window_height);
}

void Cam::Update(double dt, const vec2& player_position, bool playerMove)
{
    caminfo.camera_view.SetFramebufferSize((int)Engine::window_width, (int)Engine::window_height );
    float lerpFactor = 0.03f; // (0.0 ~ 1.0)
    vec2 target_position = player_position;
    caminfo.camera.Position.x += (target_position.x - caminfo.camera.Position.x) * lerpFactor;
    caminfo.camera.Position.y += (target_position.y - caminfo.camera.Position.y) * lerpFactor;

	world_to_cam = caminfo.camera.BuildWorldToCamera();

	// cam_to_ndc   <- get CalculateCameraToNDC from camera view
	cam_to_ndc = caminfo.camera_view.BuildCameraToNDC();

	// world_to_ndc <- cam_to_ndc * world_to_cam
	world_to_ndc = cam_to_ndc * world_to_cam;

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        LoadMap();
    }
}


void Cam::SetPosition(vec2 new_position)
{
	caminfo.camera.Position = new_position;
    
}

const vec2& Cam::GetPosition() const
{
	return caminfo.camera.Position;
}


Math::rect Cam::GetCameraBoundary() const
{
    float half_width = Engine::window_width / 2.0f;
    float half_height = Engine::window_height / 2.0f;
    float offset = 100.0f;

    return Math::rect{
        {caminfo.camera.Position.x - half_width - offset, caminfo.camera.Position.y - half_height - offset},
        {caminfo.camera.Position.x + half_width + offset, caminfo.camera.Position.y + half_height + offset}
    };
}

void Cam::LoadMap() { 
    Math::rect camera_boundary = GetCameraBoundary();
    Engine::GetGameStateManager().GetGSComponent<MapManager>()->UpdateMaps(camera_boundary);
}