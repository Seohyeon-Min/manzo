#include "Render.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "Engine.h"
#include "GameObject.h"


#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <span>
#include <array>
#include <iostream>


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

void CS230::Render::AddDrawCall(vec2 start, vec2 end , color3 color, bool iscollision) {
    if (iscollision) {
        draw_collision_calls.push_back({ start , end, color });
    }
    else {
        draw_line_calls.push_back({ start , end, color });
    }
}

// Render all draw calls
void CS230::Render::RenderAll() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& draw_call : draw_first_calls) {
        Draw(draw_call);
    }

    for (const auto& draw_call : draw_calls) {
        Draw(draw_call);
    }

    for (const auto& draw_call : draw_late_calls) {
        Draw(draw_call);
    }

    for (const auto& draw_call : draw_line_calls) {
        DrawLine(draw_call);
    }

    if (Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>() != nullptr && Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>()->Enabled()) {
        for (const auto& draw_call : draw_collision_calls) {
            DrawLine(draw_call);
        }
    }

    //std::cout << draw_calls.size() << std::endl;
    // Clear the draw call vectors if needed for the next frame
    draw_first_calls.clear();
    draw_calls.clear();
    draw_late_calls.clear();
    draw_line_calls.clear();
    draw_collision_calls.clear();
}

namespace
{
    std::span<const float, 3 * 3> to_span(const mat3& m)
    {
        return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
    }
    std::span<const float, 3> to_span(const color3& c)
    {
        return std::span<const float, 3>(&c.elements[0], 3);
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
        throw std::runtime_error("no texture!");
    }

    vec2 texture_size = (vec2)draw_call.texture->GetSize();
    mat3 model_to_world = *draw_call.transform * mat3::build_scale(texture_size);

    mat3 WORLD_TO_NDC = mat3::build_scale(2.0f / Engine::window_width, 2.0f / Engine::window_height);

    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;
    shader->SendUniform("uModelToNDC", to_span(model_to_ndc));
    model.Use();
    GLDrawIndexed(model);

    model.Use(false);
    shader->Use(false);
}

void CS230::Render::DrawLine(CollisionDrawCall drawcall)
{
    vec2 start = drawcall.start;
    vec2 end = drawcall.end;
    color3 color = drawcall.color;
    GLShader* shader = Engine::GetShaderManager().GetShader("default_collision");

    vec2 direction = end - start;
    float length = direction.Length();
    direction = direction.Normalize();

    float angle = std::atan2(direction.y, direction.x);

    mat3 model_to_world = mat3::build_translation(start) * mat3::build_rotation(angle)* mat3::build_scale(length);
    
    mat3 WORLD_TO_NDC = mat3::build_scale(2.0f / Engine::window_width, 2.0f / Engine::window_height);
    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;

    shader->Use();
    shader->SendUniform("uModelToNDC", to_span(model_to_ndc));
    shader->SendUniform("uFillColor", to_span(color));

    glCheck(glLineWidth(1.0f));
    line_model.Use();
    GLDrawVertices(line_model);

    shader->Use(false);
    line_model.Use(false);
}


void CS230::Render::CreatModel()
{
    float w = 0.5f, h = 0.5f;
    const std::array positions = { vec2{-w, -h}, vec2{w, -h}, vec2{w, h}, vec2{-w, h} };
    constexpr std::array colors = { color3{1, 1, 1}, color3{1, 0, 0}, color3{0, 1, 0}, color3{0, 0, 1} };
    constexpr std::array<unsigned, 4> indices = { 0, 3, 1, 2 };
    constexpr std::array texture_coordinates = { vec2{0, 0}, vec2{1, 0}, vec2{1, 1}, vec2{0, 1} };

    constexpr auto positions_byte_size = static_cast<long long>(sizeof(vec2) * positions.size());
    constexpr auto colors_byte_size = static_cast<long long>(sizeof(color3) * colors.size());
    constexpr auto texture_coordinates_byte_size = static_cast<long long>(sizeof(vec2) * texture_coordinates.size());
    constexpr auto buffer_size = positions_byte_size + colors_byte_size + texture_coordinates_byte_size;

    GLVertexBuffer buffer(buffer_size);
    buffer.SetData(std::span(positions));
    buffer.SetData(std::span(colors), positions_byte_size);
    buffer.SetData(std::span(texture_coordinates), positions_byte_size + colors_byte_size);

    // Position attribute
    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // Layout location for position
    position.stride = sizeof(vec2); // Stride for position
    position.offset = 0; // Offset for position

    // Color attribute
    GLAttributeLayout color;
    color.component_type = GLAttributeLayout::Float;
    color.component_dimension = GLAttributeLayout::_3;
    color.normalized = false;
    color.vertex_layout_location = 1; // Layout location for color
    color.stride = sizeof(color3); // Stride for color
    color.offset = positions_byte_size; // Offset for color

    // Texture coordinate attribute
    GLAttributeLayout texture_coord;
    texture_coord.component_type = GLAttributeLayout::Float;
    texture_coord.component_dimension = GLAttributeLayout::_2;
    texture_coord.normalized = false;
    texture_coord.vertex_layout_location = 2; // Layout location for texture coordinates
    texture_coord.stride = sizeof(vec2); // Stride for texture coordinates
    texture_coord.offset = positions_byte_size + colors_byte_size; // Offset for texture coordinates

    model.AddVertexBuffer(std::move(buffer), { position, color, texture_coord });
    model.SetPrimitivePattern(GLPrimitive::TriangleStrip);
    GLIndexBuffer index_buffer(indices);
    model.SetIndexBuffer(std::move(index_buffer));
}

void CS230::Render::CreatLineModel()
{
    const std::array positions = { vec2{0, 0}, vec2{1, 0} };

    constexpr auto positions_byte_size = static_cast<long long>(sizeof(vec2) * positions.size());
    constexpr auto buffer_size = positions_byte_size;

    GLVertexBuffer buffer(buffer_size);
    buffer.SetData(std::span(positions));

    // Position attribute
    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // Layout location for position
    position.stride = sizeof(vec2); // Stride for position
    position.offset = 0; // Offset for position

    line_model.SetVertexCount(2);
    line_model.AddVertexBuffer(std::move(buffer), { position });
    line_model.SetPrimitivePattern(GLPrimitive::Lines);
}