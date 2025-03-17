/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Camera.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once

#include "../../common_gl/include/Camera.h"
#include "../../common_gl/include/CameraView.h"
#include "GLShader.h"
#include "GLVertexArray.h"
#include "IProgram.h"
#include "angles.h"
#include "color3.h"
#include "mat3.h"
#include "Engine.h"
#include "GameObject.h"

#include <unordered_map>
#include <vector>



class Cam : public Component {
public:
    Cam();
    void Update(double dt, const vec2& player_position, bool playerMove);
    void SetPosition(vec2 DEBUG_NEW_position);
    const vec2& GetPosition() const;
    CameraView& GetCameraView() { return caminfo.camera_view; }
    Camera& GetCamera() { return caminfo.camera; }

    mat3 world_to_cam;
    mat3 cam_to_ndc;
    mat3 world_to_ndc;

    Math::rect GetBounds() const {
        vec2 view_size = caminfo.camera_view.CalcViewSizeWithZoom();
        float half_width = view_size.x / 2.0f;
        float half_height = view_size.y / 2.0f;

        vec2 position = caminfo.camera.Position;

        return {
            {position.x - half_width, position.y - half_height}, // LeftBottom
            {position.x + half_width, position.y + half_height}  // RightTop
        };
    }

private:
    float lerpFactor = 0.03f;
    vec2 target_position;

    struct CamInfo {
        Camera     camera{};
        CameraView camera_view{};
    } caminfo;
};

