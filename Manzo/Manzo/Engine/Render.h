#pragma once

#include "GLVertexArray.h"
#include "GLTexture.h"
#include "mat3.h"
#include "color3.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>

//struct DrawSettings {
//    bool enableBlending;
//    GLenum drawMode;  // GL_TRIANGLES, GL_LINES ��
//    // �߰� ���� ����
//};
//// ������Ʈ�� Draw �޼���
//void MyObject::Draw()
//{
//    // �� ������Ʈ���� ������ ��ο� ����
//    DrawSettings settings;
//    settings.enableBlending = true;   // ����: ���� Ȱ��ȭ
//    settings.drawMode = GL_TRIANGLES; // ����: �ﰢ�� ���� �׸���
//
//    Engine::GetRender().AddDrawCall({
//        sprite->GetTexture(),
//        &GetMatrix(),
//        shader,
//        settings  // ��ο� ���� ����
//        });
//}


// A structure to represent a draw call
enum class DrawLayer {
    DrawBackground,
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

        void AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase = DrawLayer::Draw);
        void AddDrawCall
        (vec2 start, vec2 end, color3 color, float width = 1.0f, float alpha = 255.0f, const GLShader* shader = nullptr, bool iscollision = true);
        void RenderAll();
        void CreatModel();
        void CreatLineModel();
        void RenderBackgrounds();

    private:
        // Internal render method
        void Draw(const DrawCall& draw_call);
        void DrawBackground(const DrawCall& draw_call);
        void DrawLine(LineDrawCall drawcall);
        void DrawLinePro(LineDrawCallPro drawcall);

        mat3 GetWorldtoNDC();


        // Vectors for draw calls
        std::vector<DrawCall> draw_background_calls;
        std::vector<DrawCall> draw_first_calls;
        std::vector<DrawCall> draw_calls;
        std::vector<DrawCall> draw_late_calls;
        std::vector<LineDrawCallPro> draw_line_calls;
        std::vector<LineDrawCall> draw_collision_calls;

        GLVertexArray model;
        GLVertexArray line_model;
    };
}