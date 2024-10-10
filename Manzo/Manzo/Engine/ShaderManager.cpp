#include "ShaderManager.h"


ShaderManager::ShaderManager() {
    default_shader = GLShader("Default Shader", {
    {GLShader::VERTEX, "assets/shaders/default2.vert"},
    {GLShader::FRAGMENT, "assets/shaders/default.frag"}
            });
}

// ���̴� �ε� �Ǵ� �̹� �����ϸ� ��ȯ
GLShader* ShaderManager::LoadShader(const std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path) {
    auto it = shaders.find(shader_name);
    if (it != shaders.end()) {
        return it->second.get();  // �̹� �ε�� ���̴� ��ȯ
    }

    // �� ���̴� �ε�
    std::unique_ptr<GLShader> shader = std::make_unique<GLShader>
        (shader_name, std::initializer_list<std::pair<GLShader::Type, std::filesystem::path>>
         { {GLShader::VERTEX,vertex_path}, {GLShader::FRAGMENT, fragment_path} });

    shaders[shader_name] = std::move(shader);
    return shaders[shader_name].get();
}

// ���̴� ��ȯ (�ε�� ���̴� �� �ϳ�)
GLShader* ShaderManager::GetShader(const std::string& shader_name) {
    auto it = shaders.find(shader_name);
    if (it != shaders.end()) {
        return it->second.get();
    }
    return nullptr;  // ���̴��� ã�� ���� ��� nullptr ��ȯ
}

GLShader* ShaderManager::GetDefaultShader()
{

    return &default_shader;
}

// ��� ���̴� ����
void ShaderManager::Clear() {
    shaders.clear();
}