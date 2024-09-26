#pragma once
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>

#include "GLTexture.h"

// A structure to represent a draw call
struct DrawCall {
    GLVertexArray* model;
    GLTexture* texture;
    mat3 transform; // You can adjust this based on your transformation representation
};

class Render {
public:
    Render() = default;

    void LoadTextureAndModel(const std::filesystem::path& texturePath, GLVertexArray* model);
    void AddDrawCall(const DrawCall& drawCall, const std::string& phase);
    void RenderAll();

private:
    // Internal render method
    void Draw(const DrawCall& draw_call);

    // Maps to store textures and models
    std::unordered_map<std::filesystem::path, GLVertexArray*> textureModelMap;
    std::vector<std::unique_ptr<GLTexture>> textures;
    std::vector<std::unique_ptr<GLVertexArray>> models;

    // Vectors for draw calls
    std::vector<DrawCall> draw_first_calls;
    std::vector<DrawCall> draw_calls;
    std::vector<DrawCall> draw_late_calls;
};