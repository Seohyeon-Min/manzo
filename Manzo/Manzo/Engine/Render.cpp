#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <span>

#include "Render.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLVertexArray.h"
#include "Engine.h"
#include "GameObject.h"

const float WORLD_SIZE_MAX = (float)std::max(Engine::window_width, Engine::window_height);

// Add a draw call to the corresponding vector based on the phase
// for each frame
void CS230::Render::AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase) {

    if (phase == DrawLayer::DrawFirst) {
        draw_first_calls.push_back(drawCall);
    }
    else if (phase == DrawLayer::DrawLast) {
        draw_late_calls.push_back(drawCall);
    }
    else {
        draw_calls.push_back(drawCall);
    }
}

// Render all draw calls
void CS230::Render::RenderAll() {
    for (const auto& draw_call : draw_first_calls) {
        Draw(draw_call);
    }

    for (const auto& draw_call : draw_calls) {
        Draw(draw_call);
    }

    for (const auto& draw_call : draw_late_calls) {
        Draw(draw_call);
    }

    // Clear the draw call vectors if needed for the next frame
    draw_first_calls.clear();
    draw_calls.clear();
    draw_late_calls.clear();
}

namespace
{
    std::span<const float, 3 * 3> to_span(const mat3& m)
    {
        return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
    }
}

// Internal render method
void CS230::Render::Draw(const DrawCall& draw_call) {
    const GLShader* shader = draw_call.shader;
    shader->Use();

    if (draw_call.texture) {
        draw_call.texture->UseForSlot(0);
        shader->SendUniform("uTex2d", 0);
    }
    else {
        throw std::runtime_error("그리기 호출에 텍스처가 제공되지 않았습니다!");
    }

    if (draw_call.model) {
        mat3 model_to_world = draw_call.transform;

        mat3 WORLD_TO_NDC;
        WORLD_TO_NDC *= WORLD_TO_NDC.build_scale(1 / WORLD_SIZE_MAX);

        const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;
        shader->SendUniform("uModelToNDC", to_span(model_to_ndc));
        draw_call.model->Use();
        GLDrawIndexed(*draw_call.model);
    }
    else {
        throw std::runtime_error("그리기 호출에 모델이 제공되지 않았습니다!");
    }
    shader->Use(false);
    draw_call.model->Use(false);
}
