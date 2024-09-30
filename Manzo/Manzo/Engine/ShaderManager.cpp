#include "ShaderManager.h"


ShaderManager::ShaderManager() {
    default_shader = GLShader("Default Shader", {
    {GLShader::VERTEX, "assets/shaders/default2.vert"},
    {GLShader::FRAGMENT, "assets/shaders/default.frag"}
            });
}

// 셰이더 로드 또는 이미 존재하면 반환
GLShader* ShaderManager::LoadShader(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path) {
    auto it = shaders.find(shader_name);
    if (it != shaders.end()) {
        return it->second.get();  // 이미 로드된 셰이더 반환
    }

    // 새 셰이더 로드
    std::unique_ptr<GLShader> shader = std::make_unique<GLShader>
        (shader_name, std::initializer_list<std::pair<GLShader::Type, std::filesystem::path>>
         { {GLShader::VERTEX,vertex_path}, {GLShader::FRAGMENT, fragment_path} });

    shaders[shader_name] = std::move(shader);
    return shaders[shader_name].get();
}

// 셰이더 반환 (로드된 셰이더 중 하나)
GLShader* ShaderManager::GetShader(const std::string& shader_name) {
    auto it = shaders.find(shader_name);
    if (it != shaders.end()) {
        return it->second.get();
    }
    return nullptr;  // 셰이더를 찾지 못한 경우 nullptr 반환
}

GLShader* ShaderManager::GetDefaultShader()
{

    return &default_shader;
}

// 모든 셰이더 제거
void ShaderManager::Clear() {
    shaders.clear();
}