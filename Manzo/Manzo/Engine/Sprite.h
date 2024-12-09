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

#include "Animation.h"
#include "Component.h"
#include "Collision.h"
#include "engine.h"

 
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
    //void Draw();
    ivec2 GetHotSpot(int index);
    ivec2 GetFrameSize() { return frame_size; };
    GLTexture* GetTexture() { return texture; }
    ivec2 GetFrameTexel(int index) const;
    const int GetCurrentFrame() { return animations[current_animation]->CurrentFrame(); }
    const std::filesystem::path& GetFilePath() const { return sprite_file_path; }
    const int CurrentAnimation() { return current_animation; }
    void PlayAnimation(int animation);
    bool AnimationEnded();
    void Reset();

private:
    GLTexture* texture;
    std::vector<ivec2> hotspots;

    int current_animation;
    ivec2 frame_size;
    std::filesystem::path sprite_file_path;
    std::vector<ivec2> frame_texels;
    std::vector<Animation*> animations;
};
