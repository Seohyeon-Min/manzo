#pragma once

#include "GLShader.h"
#include <unordered_map>
#include <string>
#include <memory>

class ShaderManager {
public:
    ShaderManager();
    GLShader* LoadShader(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path);
    GLShader* GetShader(const std::string& shader_name);
    GLShader* GetDefaultShader();

    void Clear();

private:
    std::unordered_map<std::string, std::shared_ptr<GLShader>> shaders;
    std::shared_ptr<GLShader> default_shader;
};