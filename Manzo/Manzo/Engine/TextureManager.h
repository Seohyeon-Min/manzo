
#pragma once
#include "GLTexture.h"
#include "GLVertexArray.h"

#include <filesystem>
#include <map>

static const GLShader basic_shader = GLShader(
    "Basic Shader", { 
    {GLShader::VERTEX, "assets/shaders/tutorial-5.vert"}, 
    {GLShader::FRAGMENT, "assets/shaders/tutorial-5.frag"} });

namespace CS230 {

    class TextureManager {
    public:
        TextureManager() { shaders.push_back(&basic_shader); }
        GLTexture* Load(const std::filesystem::path& file_name, const GLShader* shader = &basic_shader);
        void Unload();
        GLVertexArray* CreatModel(const float width, const float height);
        //void StartRenderTextureMode(int width, int height);
        //GLTexture* EndRenderTextureMode();

    private:
        float ConvertToNDCWidth(int width, int screen_width) {
            return (2.0f * width) / screen_width - 1.0f; // -1.0f ~ 1.0f 범위로 변환
        }

        float ConvertToNDCHeight(int height, int screen_height) {
            return (2.0f * height) / screen_height - 1.0f; // -1.0f ~ 1.0f 범위로 변환
        }
        std::map<std::filesystem::path, GLTexture*> textures;
        std::vector<const GLShader*> shaders;
    };
}



