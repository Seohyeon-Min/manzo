

#include "TextureManager.h"

#include "Engine.h"
#include "color3.h"
#include "Render.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include  <stb_image.h>

GLTexture* TextureManager::Load(const std::filesystem::path& file_name)
{
    auto texture_iterator = textures.find(file_name);
    if (texture_iterator != textures.end()) {
        return textures[file_name];
    }
    else {
        GLTexture* new_texture = new GLTexture(std::filesystem::path(file_name));
        //new_texture->SetShader(shader);
        textures.insert(std::make_pair(file_name, new_texture));
        Engine::GetLogger().LogEvent("Loading Texture: " + file_name.string());
        return textures[file_name];
    }
}

void TextureManager::Unload()
{
    for (auto& pair : textures) {
        delete pair.second;
    }
    textures.clear();
    Engine::GetLogger().LogEvent("Clearing Textures");
}