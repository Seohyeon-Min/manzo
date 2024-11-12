#pragma once

#include "GLVertexArray.h"
#include "GLTexture.h"
#include "mat3.h"
#include "color3.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <functional>
//struct DrawSettings {
//    bool enableBlending;
//    GLenum drawMode;  // GL_TRIANGLES, GL_LINES 등
//    // 추가 설정 가능
//};
//// 오브젝트의 Draw 메서드
//void MyObject::Draw()
//{
//    // 각 오브젝트마다 고유한 드로우 설정
//    DrawSettings settings;
//    settings.enableBlending = true;   // 예시: 블렌딩 활성화
//    settings.drawMode = GL_TRIANGLES; // 예시: 삼각형 모드로 그리기
//
//    Engine::GetRender().AddDrawCall({
//        sprite->GetTexture(),
//        &GetMatrix(),
//        shader,
//        settings  // 드로우 설정 전달
//        });
//}


// A structure to represent a draw call
enum class DrawLayer {
    DrawBackground,
    DrawFirst,
    Draw,
    DrawLast,
    DrawUI
};

namespace CS230 {

    struct DrawSettings
    {
        int                   filtering_index = 0;
        std::array<int, 2>    wrap_index = { 0, 0 };
        float                 tex_coord_scale = 1.0f;
        bool                  modulate_color = false;
        bool                  apply_procedural_texture = false;
        float                 procedural_tile_size = 8.0f;
        bool                  use_texture = true;
        bool                  do_blending = false;
        bool                  animate_procedural_texture = false;
    } settings;

    struct DrawCall {
        GLTexture* texture;
        const mat3* transform;
        const GLShader* shader;
        std::function<void(const GLShader*)> SetUniforms = nullptr;
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

    class Render {
    public:
        Render() { CreatModel(); CreatLineModel(); };

        void AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase = DrawLayer::Draw, DrawSettings setting = settings);
        void AddDrawCall
        (vec2 start, vec2 end, color3 color, float width = 1.0f, float alpha = 255.0f, const GLShader* shader = nullptr, bool iscollision = true);
        void RenderAll();
        void CreatModel();
        void CreatLineModel();
        void RenderBackgrounds();
        void ClearDrawCalls();

    private:
        // Internal render method
        void Draw(const DrawCall& draw_call, DrawSettings setting = settings, bool isUI = false);
        void DrawBackground(const DrawCall& draw_call);
        void DrawLine(LineDrawCall drawcall);
        void DrawLinePro(LineDrawCallPro drawcall);

        mat3 GetWorldtoNDC();


        // Vectors for draw calls
        std::vector<DrawCall> draw_background_calls;
        std::vector<DrawCall> draw_first_calls;
        std::vector<DrawCall> draw_calls;
        std::vector<DrawCall> draw_late_calls;
        std::vector<DrawCall> draw_ui_calls;
        std::vector<LineDrawCallPro> draw_line_calls;
        std::vector<LineDrawCall> draw_collision_calls;

        GLVertexArray model;
        GLVertexArray line_model;
    };
}