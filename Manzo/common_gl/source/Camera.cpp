/*!***********************************************************************
* File: Camera.cpp
* Author: Gyuwon Na
* Course: CS200
* Last Updated: 09-26-2024
* Brief
	- Calculation about camera
**************************************************************************/
#include "Camera.h"
#include <corecrt_math.h>

/*!***********************************************************************
* brief		constructor
* param		vec2 camera_position (set camera position)
			vec2 camera_up (set direction of camera)
*************************************************************************/
Camera::Camera(vec2 camera_position, vec2 camera_up) noexcept
{
	Position = camera_position;
	IsFirstPerson = false;
	up = camera_up;
	right = vec2{ up.y, -up.x };
	orientation = static_cast<float>(atan2(up.y, up.x));
}

/*!***********************************************************************
* brief		set direction of camera
* param		vec2 camera_up (set direction of camera)
*************************************************************************/
void Camera::SetUp(vec2 camera_up) noexcept
{
	up = camera_up;
	right = vec2{ up.y, -up.x };
	orientation = static_cast<float>(atan2(up.y, up.x));
}

/*!***********************************************************************
* brief		set direction of camera
* param		vec2 camera_up (set direction of camera)
*************************************************************************/
void Camera::SetOrientation(float angle_radians) noexcept
{
	orientation = angle_radians;
	up = vec2{ static_cast<float>(cos(orientation)), static_cast<float>(sin(orientation)) };
	right = vec2{ up.y, -up.x };
}

/*!***********************************************************************
* brief		moving camera up
* param		float distance (add distance)
*************************************************************************/
void Camera::MoveUp(float distance) noexcept
{
	Position.x += up.x * distance;
	Position.y += up.y * distance;
}

/*!***********************************************************************
* brief		moving camera right
* param		float distance (add distance)
*************************************************************************/
void Camera::MoveRight(float distance) noexcept
{
	Position.x += right.x * distance;
	Position.y += right.y * distance;
}

/*!***********************************************************************
* brief		reflect camera rotation
* param		float angle_radians (rotation degree)
*************************************************************************/
void Camera::Rotate(float angle_radians) noexcept
{
	SetOrientation(orientation + angle_radians);
}

/*!***********************************************************************
* brief		calculate camera to world matrix
*************************************************************************/
mat3 Camera::BuildCameraToWorld() const noexcept
{
	if (IsFirstPerson)
	{
		// build and return camera to world matrix using our up, right and position vectors
		return { {right.x,right.y,0},{up.x,up.y,0},{Position.x,Position.y,1} };
	}
	else
	{
		// build and return camera to world matrix using only our position vector
		return { {1,0,0},{0,1,0},{Position.x,Position.y,1} };
	}
}

/*!***********************************************************************
* brief		calculate world to camera matrix
*************************************************************************/
mat3 Camera::BuildWorldToCamera() const noexcept
{
	if (IsFirstPerson)
	{
		// build and return world to camera matrix using our up, right and position vectors
		float cx = -1.f * (right.x * Position.x + right.y * Position.y);
		float cy = -1.f * (up.x * Position.x + up.y * Position.y);
		return { {right.x,up.x,0},{right.y,up.y,0},{cx,cy,1} };
	}
	else
	{
		// build and return world to camera matrix using only our position vector
		return { {1,0,0},{0,1,0},{-Position.x,-Position.y,1} };
	}
}
