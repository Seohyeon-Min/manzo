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
#include <iostream>

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


void Camera::StartShake(float intensity, float duration) noexcept
{
	shake_intensity = intensity;
	shake_duration = duration;
}

void Camera::UpdateShake(float dt) noexcept
{
	if (shake_duration > 0.0f) {
		shake_duration -= dt; // 흔들림 지속 시간 감소

		// 흔들림 강도를 점진적으로 감소 (감쇠 효과)
		shake_intensity *= 0.9f;

		if (shake_duration <= 0.0f) {
			shake_intensity = 0.0f; // 흔들림 종료
		}
	}
}
vec2 Camera::GetShakingPosition() const noexcept
{
	if (shake_intensity > 0.0f && shake_duration > 0.0f) {
		// 랜덤 오프셋 생성
		float offsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shake_intensity;
		float offsetY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f * shake_intensity;

		// 원래 카메라 위치에 흔들림 오프셋 추가
		return Position + vec2{ offsetX, offsetY };
	}
	return Position; // 흔들림이 없으면 원래 위치 반환
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
void Camera::MoveUp(float distance, float dt, float smoothFactor) noexcept
{
	vec2 targetPosition = Position + up * distance;

	Position.x += (targetPosition.x - Position.x) * smoothFactor * dt;
	Position.y += (targetPosition.y - Position.y) * smoothFactor * dt;
}

/*!***********************************************************************
* brief		moving camera right
* param		float distance (add distance)
*************************************************************************/
void Camera::MoveRight(float distance, float dt, float smoothFactor) noexcept
{
	vec2 targetPosition = Position + right * distance;

	Position.x += (targetPosition.x - Position.x) * smoothFactor * dt;
	Position.y += (targetPosition.y - Position.y) * smoothFactor * dt;
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
	vec2 shaking_position = GetShakingPosition(); // 흔들림 적용된 위치 사용
	if (IsFirstPerson) {
		std::cout << "asdjfasdf" << shaking_position.x << std::endl;
		return { {right.x, right.y, 0}, {up.x, up.y, 0}, {shaking_position.x, shaking_position.y, 1} };
	}
	else {
		return { {1, 0, 0}, {0, 1, 0}, {shaking_position.x, shaking_position.y, 1} };
	}
}
/*!***********************************************************************
* brief		calculate world to camera matrix
*************************************************************************/
mat3 Camera::BuildWorldToCamera() const noexcept
{
	vec2 shaking_position = GetShakingPosition();
	if (IsFirstPerson)
	{
		// build and return world to camera matrix using our up, right and position vectors
		float cx = -1.f * (right.x * shaking_position.x + right.y * shaking_position.y);
		float cy = -1.f * (up.x * shaking_position.x + up.y * shaking_position.y);
		return { {right.x,up.x,0},{right.y,up.y,0},{cx,cy,1} };
	}
	else
	{
		// build and return world to camera matrix using only our position vector
		return { {1,0,0},{0,1,0},{-shaking_position.x,-shaking_position.y,1} };
	}
}
