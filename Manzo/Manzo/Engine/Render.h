#pragma once

#include "GLVertexArray.h"
#include "GLTexture.h"
#include "mat3.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>

// A structure to represent a draw call
enum class DrawLayer {
    DrawFirst,
    Draw,
    DrawLast
};

namespace CS230 {
    struct DrawCall {
        GLVertexArray* model;
        GLTexture* texture;
        mat3 transform;
    };

    class Render {
    public:

        Render() = default;

        void LoadTextureAndModel(const std::filesystem::path& texturePath, GLVertexArray* model);
        void AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase = DrawLayer::Draw);
        void RenderAll();

    private:
        // Internal render method


        void Draw(const DrawCall& draw_call);

        // Vectors for draw calls
        std::vector<DrawCall> draw_first_calls;
        std::vector<DrawCall> draw_calls;
        std::vector<DrawCall> draw_late_calls;
        GLShader basic_shader;
    };
}