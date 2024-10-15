#pragma once

#include "GLVertexArray.h"
#include "GLTexture.h"
#include "mat3.h"
#include "color3.h"

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
        GLTexture* texture;
        const mat3* transform;
        const GLShader* shader;
    };

    struct CollisionDrawCall {
        vec2 start;
        vec2 end;
        color3 color;
    };

    class Render {
    public:
        Render() { CreatModel(); CreatLineModel(); };

        void AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase = DrawLayer::Draw);
        void AddDrawCall(vec2 start, vec2 end, color3 color);
        void RenderAll();
        void CreatModel();
        void CreatLineModel();
        void DrawBackground(const DrawCall& draw_call);

    private:
        // Internal render method
        void Draw(const DrawCall& draw_call);
        void DrawLine(CollisionDrawCall drawcall);

        mat3 GetWorldtoNDC();

        // Vectors for draw calls
        std::vector<DrawCall> draw_first_calls;
        std::vector<DrawCall> draw_calls;
        std::vector<DrawCall> draw_late_calls;
        std::vector<CollisionDrawCall> draw_collision_calls;

        GLVertexArray model;
        GLVertexArray line_model;
    };
}