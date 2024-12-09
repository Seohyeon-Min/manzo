
#pragma once
#include "GLTexture.h"
#include "GLVertexArray.h"

#include <filesystem>
#include <map>
//
//static const GLShader basic_shader = GLShader(
//    "Basic Shader", { 
//    {GLShader::VERTEX, "assets/shaders/tutorial-5.vert"}, 
//    {GLShader::FRAGMENT, "assets/shaders/tutorial-5.frag"} });



class TextureManager {
public:
    GLTexture* Load(const std::filesystem::path& file_name);
    void Unload();
    GLVertexArray* GetModel(const std::filesystem::path& file_name);

private:
    std::map<std::filesystem::path, GLTexture*> textures;
    std::vector<const GLShader*> shaders;
};
