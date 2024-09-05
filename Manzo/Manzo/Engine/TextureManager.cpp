/*
Copyright (C) 2023 DigiPen Institute of Technology
Reproduction or distribution of this file or its contents without
prior written consent is prohibited
File Name:  TextureManager.cpp
Project:    CS230 Engine
Author:     Seohyeon Min
Created:    Aprill 30, 2023
*/


#include "TextureManager.h"
#include "Engine.h"
#include "Texture.h"

namespace {
    RenderTexture2D s_render_texture = {};
}

void CS230::TextureManager::StartRenderTextureMode(int width, int height) {
    s_render_texture = LoadRenderTexture(width, height);
    BeginTextureMode(s_render_texture);
    rlClearColor(0, 0, 0, 0);
    rlClearScreenBuffers();
}

CS230::Texture* CS230::TextureManager::EndRenderTextureMode() {
    EndTextureMode();
    const auto window_size = Engine::GetWindow().GetSize();
    rlSetFramebufferWidth(window_size.x);
    rlSetFramebufferHeight(window_size.y);
    rlUnloadFramebuffer(s_render_texture.id);
    const auto result = new Texture{ s_render_texture.texture };
    rendered_textures.push_back(result);
    return result;
}

CS230::Texture* CS230::TextureManager::Load(const std::filesystem::path& file_name)
{
    auto texture_iterator = textures.find(file_name);
    if (texture_iterator != textures.end()) {
        return textures[file_name];
    }
    else {
        Texture* new_texture = new Texture(std::filesystem::path(file_name));
        textures.insert(std::make_pair(file_name, new_texture));
        Engine::GetLogger().LogEvent("Loading Texture: " + file_name.string());
        return textures[file_name];
    }
}

void CS230::TextureManager::Unload()
{
    for (auto& pair : textures) {
        delete pair.second;
    }
    textures.clear();
	Engine::GetLogger().LogEvent("Clearing Textures");
}
