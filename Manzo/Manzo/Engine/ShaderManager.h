#pragma once

#include "GLShader.h"
#include <unordered_map>
#include <string>
#include <memory>

class ShaderManager {
public:
    // 셰이더 로드 또는 이미 존재하면 반환
    ShaderManager();
    GLShader* LoadShader(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path);
    GLShader* GetShader(const std::string& shader_name);
    GLShader* GetDefaultShader();

    // 모든 셰이더 제거
    void Clear();

private:
    // 셰이더 저장을 위한 맵
    std::unordered_map<std::string, std::unique_ptr<GLShader>> shaders;
    GLShader default_shader;
};