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



class Sprite;

struct DrawCall {
    std::variant<Sprite*, GLTexture*> drawable;
    const mat3* transform;
    const GLShader* shader;
    std::function<void(const GLShader*)> SetUniforms = nullptr;
    DrawSettings settings; // later, change it to pointer
};

struct LineDrawCall {
    vec2 start;
    vec2 end;
    color3 color;
    const GLShader* shader;
};

struct LineDrawCallPro {
    vec2 start;
    vec2 end;
    color3 color;
    const float width;
    const float alpha;
    const GLShader* shader;
};

struct CircleDrawCall {
    float radius;
    vec2 pos;
    const GLShader* shader;
    std::function<void(const GLShader*)> SetUniforms = nullptr;
    DrawSettings settings;
};

class Render {
public:
    Render();

    // 매개변수를 스트럭트로 넘길수도 있다!!! 그게 더 좋을거같다!!!

    void AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase = DrawLayer::Draw);
    void AddDrawCall
    (vec2 start, vec2 end, color3 color, float width = 1.0f, float alpha = 255.0f, const GLShader* shader = nullptr, bool iscollision = true);
    void AddDrawCall(const CircleDrawCall& drawcall, const DrawLayer& phase = DrawLayer::Draw);
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


    // Vectors for draw calls
    // z 발류 갖고있는 스트럭트로 불투명 리스트 하나, 투명 리스트 하나 할 수 있다!!
    // 이게 더 좋을듯

    std::vector<DrawCall> draw_background_calls;
    std::vector<DrawCall> draw_first_calls;
    std::vector<DrawCall> draw_calls;
    std::vector<DrawCall> draw_late_calls;
    std::vector<DrawCall> draw_ui_calls;
    std::vector<DrawCall> draw_dialog_calls;
    std::vector<LineDrawCallPro> draw_line_calls;
    std::vector<LineDrawCall> draw_collision_calls;
    std::vector<CircleDrawCall> draw_circle_calls;

    GLVertexArray model;
    GLVertexArray line_model;
    GLVertexArray circle_line_model;
    GLFrameBuffer postProcessFramebuffer;

    mat3 projection_matrix;
};
