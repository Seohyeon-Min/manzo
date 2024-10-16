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


namespace CS230 {
    class Cam : public Component{
    public:
        Cam();
        void Update(double dt, const vec2& player_position, bool playerMove);
        void SetPosition(vec2 new_position);
        const vec2& GetPosition() const;
        void SetLimit(Math::rect new_limit);
        const CameraView GetCameraView() { return caminfo.camera_view; }
        const Camera GetCamera() { return caminfo.camera; }

        mat3 world_to_cam;
        mat3 cam_to_ndc;
        mat3 world_to_ndc;

    private:
        Math::rect limit;

        struct CamInfo
        {
            Camera     camera{};
            CameraView camera_view{};
            float      move_scalar = 0;
            float      turn_scalar = 0;
            float      strafe_scalar = 0;
            float      move_speed = 240.0f;
        } caminfo;
    };
}
