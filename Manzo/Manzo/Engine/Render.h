#pragma once

#include "GLVertexArray.h"
#include "GLTexture.h"
#include "mat3.h"
#include "color3.h"
#include "DrawSetting.h"
#include "GLFrameBuffer.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <variant>

// A structure to represent a draw call
enum class DrawLayer {
    DrawBackground,
    DrawFirst,
    Draw,
    DrawLast,
    DrawUI,
    DrawDialog
};

enum class DrawType {
    Regular,
    Line,
    Circle
};

class Sprite;

struct BaseDrawCall {
    const GLShader* shader = nullptr;
    DrawLayer sorting_layer = DrawLayer::Draw;
    int order_in_layer = 0; 
    std::function<void(const GLShader*)> SetUniforms = nullptr; 
    DrawSettings settings;

    virtual ~BaseDrawCall() = default;
    virtual DrawType GetDrawType() const = 0;
};

struct DrawCall : BaseDrawCall {
    std::variant<Sprite*, GLTexture*> drawable;
    const mat3* transform = nullptr;
    const GLShader* shader = nullptr;

    DrawCall() = default;

    DrawCall(Sprite* sprite, const mat3* transform_matrix, const GLShader* shader_ptr = nullptr)
        : drawable(sprite), transform(transform_matrix) {
        shader = shader_ptr;
    }

    DrawCall(GLTexture* texture, const mat3* transform_matrix, const GLShader* shader_ptr = nullptr)
        : drawable(texture), transform(transform_matrix) {
        shader = shader_ptr;
    }

    DrawType GetDrawType() const override {
        return DrawType::Regular;
    }
};

struct LineDrawCall : BaseDrawCall {
    vec2 start;
    vec2 end;
    color3 color;

    DrawType GetDrawType() const override {
        return DrawType::Line;
    }
};

struct LineDrawCallPro : BaseDrawCall {
    vec2 start;
    vec2 end;
    color3 color;
    float width = 1.0f;
    float alpha = 255.0f;
    DrawType draw_type = DrawType::Line;

    DrawType GetDrawType() const override {
        return DrawType::Line;
    }
};

struct CircleDrawCall : BaseDrawCall {
    float radius = 0.0f;
    vec2 pos;
    DrawType draw_type = DrawType::Circle;

    CircleDrawCall() = default;
    CircleDrawCall(float radius, vec2 pos) : radius(radius), pos(pos) {}

    DrawType GetDrawType() const override {
        return DrawType::Circle;
    }
};

class Render {
public:
    Render();

    void AddDrawCall(std::unique_ptr<BaseDrawCall> drawCall);
    void AddBackgroundDrawCall(const DrawCall& drawCall);
    void AddDrawCall
    (vec2 start, vec2 end, color3 color, float width = 1.0f, float alpha = 255.0f, const GLShader* shader = nullptr, bool iscollision = true);
    void RenderAll();

    void ApplyPostProcessing();
    void RenderQuad();

    void SetProjection(const mat3 proj);
    void CreatModel();
    void CreatLineModel();
    void CreateCircleLineModel();
    void ClearDrawCalls();

private:
    // Internal render method
    void Draw(const DrawCall& draw_call);
    void DrawBackground(const DrawCall& draw_call);
    void DrawLine(LineDrawCall drawcall);
    void DrawLinePro(LineDrawCallPro drawcall);
    void DrawCircleLine(CircleDrawCall drawcall);

    mat3 GetWorldtoNDC();

    std::vector<std::unique_ptr<BaseDrawCall>> all_draw_calls;
    std::vector<DrawCall> draw_background_calls;
    std::vector<std::unique_ptr<DrawCall>> draw_calls;  
    std::vector<LineDrawCall> draw_collision_calls;

    GLVertexArray model;
    GLVertexArray line_model;
    GLVertexArray circle_line_model;
    GLFrameBuffer postProcessFramebuffer;

    mat3 projection_matrix;
};
