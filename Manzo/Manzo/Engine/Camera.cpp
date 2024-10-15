#include "Camera.h"

CS230::Cam::Cam()
{
	caminfo.camera_view.SetFramebufferSize(Engine::window_width, Engine::window_height);
}

void CS230::Cam::Update(double dt)
{
	caminfo.camera.MoveUp(caminfo.move_speed * (float)dt * caminfo.move_scalar);
	caminfo.camera.MoveRight(caminfo.move_speed * (float)dt * caminfo.strafe_scalar);

	mat3 world_to_cam = caminfo.camera.BuildWorldToCamera();

	// cam_to_ndc   <- get CalculateCameraToNDC from camera view
	mat3 cam_to_ndc = caminfo.camera_view.BuildCameraToNDC();

	// world_to_ndc <- cam_to_ndc * world_to_cam
	mat3 world_to_ndc = cam_to_ndc * world_to_cam;
}

void CS230::Cam::SetPosition(vec2 new_position)
{
	caminfo.camera.Position = new_position;
}

const vec2& CS230::Cam::GetPosition() const
{
	return caminfo.camera.Position;
}
