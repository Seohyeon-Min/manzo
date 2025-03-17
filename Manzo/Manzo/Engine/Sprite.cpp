/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  Sprite.cpp
Project:    CS230 Engine
Author:     Seohyeon Min
Created:    March 22, 2023
*/

#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)

#include "Sprite.h"

Sprite::Sprite(const std::filesystem::path& sprite_file, GameObject* given_object) {
    Load(sprite_file, given_object);
}

Sprite::Sprite(Sprite&& temporary) noexcept :
    texture(std::move(temporary.texture)),
    hotspots(std::move(temporary.hotspots)),
    current_animation(temporary.current_animation),
    frame_size(temporary.frame_size),
    frame_texels(std::move(temporary.frame_texels)),
    animations(std::move(temporary.animations))
{ }

Sprite& Sprite::operator=(Sprite&& temporary) noexcept {
    std::swap(texture, temporary.texture);
    std::swap(hotspots, temporary.hotspots);
    std::swap(current_animation, temporary.current_animation);
    std::swap(frame_size, temporary.frame_size);
    std::swap(frame_texels, frame_texels);
    std::swap(animations, temporary.animations);
    return *this;
}

Sprite::~Sprite()
{
    for (Animation* animation : animations) {
        delete animation;
    }
    animations.clear();
}

void Sprite::Load(const std::filesystem::path& sprite_file, GameObject* object) {
    

    if (sprite_file.extension() != ".spt") {
        throw std::runtime_error(sprite_file.generic_string() + " is not a .spt file");
    }
    std::ifstream in_file(sprite_file);

    if (in_file.is_open() == false) {
        throw std::runtime_error("Failed to load " + sprite_file.generic_string());
    }

    hotspots.clear();
    frame_texels.clear();
    animations.clear();

    std::string text;
    std::string anim_file_path;
    in_file >> text;
    sprite_file_path = text;
    texture = Engine::GetTextureManager().Load(text); // add shader by parameter?
    frame_size = texture->GetSize();

    in_file >> text;
    while (in_file.eof() == false) {
        if (text == "FrameSize") {
            in_file >> frame_size.x;
            in_file >> frame_size.y;
        }
        else if (text == "NumFrames") {
            int frame_count;
            in_file >> frame_count;
            for (int i = 0; i < frame_count; i++) {
                frame_texels.push_back({ frame_size.x * i, 0 });
            }
        }
        else if (text == "Frame") {
            int frame_location_x, frame_location_y;
            in_file >> frame_location_x;
            in_file >> frame_location_y;
            frame_texels.push_back({ frame_location_x, frame_location_y });
        }
        else if (text == "HotSpot") {
            int hotspot_x, hotspot_y;
            in_file >> hotspot_x;
            in_file >> hotspot_y;
            hotspots.push_back({ hotspot_x, hotspot_y });
        }
        else if (text == "Anim") {
            in_file >> anim_file_path;
            Animation* animation = DEBUG_NEW Animation(std::filesystem::path(anim_file_path));
            animations.push_back( animation );
        }
        else if (text == "RectCollision") {
            Math::irect boundary;
            in_file >> boundary.point_1.x >> boundary.point_1.y >> boundary.point_2.x >> boundary.point_2.y;
            if (object == nullptr) {
                Engine::GetLogger().LogError("Cannot add collision to a null object");
            }
            else {
                object->AddGOComponent(DEBUG_NEW RectCollision(boundary, object));
            }
        }
        else {
            Engine::GetLogger().LogError("Unknown command: " + text);
        }
        in_file >> text;
    }
    if (frame_texels.empty() == true) {
        frame_texels.push_back({ 0,0 });
    }
    if (animations.empty() == true) {
        Animation* animation = DEBUG_NEW Animation();
        animations.push_back(animation);
        PlayAnimation(0);
    }
}

void Sprite::Update(double dt)
{
    animations[current_animation]->Update(dt);

}

ivec2 Sprite::GetHotSpot(int index)
{
	if (index < 0 || index >= hotspots.size()) {
		//Engine::GetLogger().LogError("Error: wrong hotspot index.");
		return ivec2({0,0});
	}
	return ivec2(hotspots[index]);
}

void Sprite::PlayAnimation(int animation)
{
    if (animation < 0 || animation > animations.size()) {
        Engine::GetLogger().LogError("the animation doesn't exist.");
        current_animation = 0;
    }
    else {
        current_animation = animation;
        animations[current_animation]->Reset();
    }
}

bool Sprite::AnimationEnded()
{
    if (animations[current_animation]->Ended())
        return true;
    else
        return false;
}

void Sprite::Reset()
{
    animations[current_animation]->Reset();
}

ivec2 Sprite::GetFrameTexel(int index) const
{
	if (index < 0 || index >= frame_texels.size()) {
		return ivec2({ 0,0 });
	}
	return ivec2(frame_texels[index]);
}

//void Sprite::Draw() {
//    //texture->Draw(display_matrix * mat3(-GetHotSpot(0)), GetFrameTexel(animations[current_animation]->CurrentFrame()), GetFrameSize());
//
//}
