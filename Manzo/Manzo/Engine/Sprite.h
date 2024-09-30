/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Sprite.h
Project:    CS230 Engine
Author:     Jonathan Holmes
Created:    March 8, 2023
*/

#pragma once
#include "GameObject.h"
#include <string>
#include "Texture.h"
#include "Animation.h"
#include "Component.h"
#include "Collision.h"
#include "engine.h"

namespace CS230 {
    class Sprite :public Component{
    public:
        Sprite(const std::filesystem::path& sprite_file, GameObject* given_object);
        ~Sprite();

        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;

        Sprite(Sprite&& temporary) noexcept;
        Sprite& operator=(Sprite&& temporary) noexcept;
        void Update(double dt) override;
        void Load(const std::filesystem::path& sprite_file, GameObject* given_object);
        void Draw(Math::TransformationMatrix display_matrix);
        Math::ivec2 GetHotSpot(int index);
        Math::ivec2 GetFrameSize() { return frame_size; };
        const int CurrentAnimation() { return current_animation; }
        void PlayAnimation(int animation);
        bool AnimationEnded();
        void Reset();
    private:
        Math::ivec2 GetFrameTexel(int index) const;

        Texture* texture;
        std::vector<Math::ivec2> hotspots;

        int current_animation;
        Math::ivec2 frame_size;
        std::vector<Math::ivec2> frame_texels;
        std::vector<Animation*> animations;
    };
}
