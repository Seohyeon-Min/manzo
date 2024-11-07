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
#include "Camera.h"


const float WORLD_SIZE_MAX = (float)std::max(Engine::window_width, Engine::window_height);

// Add a draw call to the corresponding vector based on the draw layer
// Draw calls are grouped into first, normal, and late phases
void CS230::Render::AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase) {
    if (phase == DrawLayer::DrawFirst) {
        draw_first_calls.push_back(drawCall); // Add to early phase
    }
    else if (phase == DrawLayer::DrawLast) {
        draw_late_calls.push_back(drawCall); // Add to late phase
    }
    else if (phase == DrawLayer::DrawBackground)
    {
        draw_background_calls.push_back(drawCall);
    }
    else {
        draw_calls.push_back(drawCall); // Add to normal phase
    }
}

// Overloaded function to add a line or collision draw call
// depending on whether it is a collision line
void CS230::Render::AddDrawCall
(vec2 start, vec2 end, color3 color,float width, float alpha ,const GLShader* shader, bool iscollision) {
    if (iscollision) {
        draw_collision_calls.push_back({ start, end, color, shader }); // Collision line
    }
    else {
        draw_line_calls.push_back({ start, end, color, width, alpha, shader }); // Regular line
    }
}

// Render all stored draw calls, starting with early phase, normal phase, and then late phase
// Also handles rendering of lines and collision shapes
void CS230::Render::RenderAll() {
    // Draw calls in the early phase
    for (const auto& draw_call : draw_first_calls) {
        Draw(draw_call);
    }

    // Draw normal draw calls
    for (const auto& draw_call : draw_calls) {
        Draw(draw_call);
    }

    // Draw calls in the late phase
    for (const auto& draw_call : draw_late_calls) {
        Draw(draw_call);
    }

    // Draw lines
    float line_width = 2.0f;
    for (const auto& draw_call : draw_line_calls) {
        DrawLinePro(draw_call);
    }

    // If collision debug mode is enabled, render collision lines
    if (Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>() != nullptr &&
        Engine::GetGameStateManager().GetGSComponent<CS230::ShowCollision>()->Enabled()) {
        for (const auto& draw_call : draw_collision_calls) {
            DrawLine(draw_call); // Collision lines
        }
    }

    // Clear draw call vectors for the next frame
    draw_first_calls.clear();
    draw_calls.clear();
    draw_late_calls.clear();
    draw_line_calls.clear();
    draw_collision_calls.clear();
}

void CS230::Render::RenderBackgrounds()
{
    for (const auto& draw_call : draw_background_calls) {
        Draw(draw_call);
    }

    draw_background_calls.clear();
}

// Helper function to convert matrix or color to a span of floats
namespace {
    std::span<const float, 3 * 3> to_span(const mat3& m) {
        return std::span<const float, 3 * 3>(&m.elements[0][0], 9);
    }
    std::span<const float, 3> to_span(const color3& c) {
        return std::span<const float, 3>(&c.elements[0], 3);
    }
}

// Draw an individual draw call (textured quad)
// Converts world coordinates to normalized device coordinates (NDC)
void CS230::Render::Draw(const DrawCall& draw_call) {
    const GLShader* shader = draw_call.shader;
    shader->Use(); // Use the specified shader

    // Ensure the texture is valid, then use it and send it to the shader
    if (draw_call.texture) {
        draw_call.texture->UseForSlot(0);
        shader->SendUniform("uTex2d", 0);
    }
    else {
        throw std::runtime_error("no texture!"); // Error if no texture is assigned
    }

    vec2 texture_size = (vec2)draw_call.texture->GetSize();
    mat3 model_to_world = *draw_call.transform * mat3::build_scale(texture_size); // Scale the model based on texture size

    mat3 WORLD_TO_NDC = GetWorldtoNDC();

    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;

    shader->SendUniform("uModelToNDC", to_span(model_to_ndc)); // Send transformation matrix to shader
    model.Use(); // Bind the model for drawing
    GLDrawIndexed(model); // Draw the model

    model.Use(false); // Unbind the model
    shader->Use(false); // Unbind the shader
}

// Draw a line between two points for collision or debugging purposes
void CS230::Render::DrawLine(LineDrawCall drawcall) {
    vec2 start = drawcall.start;
    vec2 end = drawcall.end;
    color3 color = drawcall.color;
    const GLShader* shader = drawcall.shader;

    // Use default collision shader if no shader is provided
    if (shader == nullptr) {
        shader = Engine::GetShaderManager().GetShader("default_collision");
    }

    vec2 direction = end - start;
    float length = direction.Length(); // Calculate length of the line
    direction = direction.Normalize(); // Normalize direction vector

    float angle = std::atan2(direction.y, direction.x); // Calculate angle of the line

    // Build transformation matrix for the line
    mat3 model_to_world = mat3::build_translation(start) * mat3::build_rotation(angle) * mat3::build_scale(length);

    // Convert to NDC coordinates
    mat3 WORLD_TO_NDC = GetWorldtoNDC();
    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;

    shader->Use(); // Use shader
    shader->SendUniform("uModelToNDC", to_span(model_to_ndc)); // Send transformation matrix to shader
    shader->SendUniform("uFillColor", to_span(color)); // Send line color to shader

    line_model.Use(); // Bind line model
    GLDrawVertices(line_model); // Draw the line

    shader->Use(false); // Unbind shader
    line_model.Use(false); // Unbind line model
}

void CS230::Render::DrawLinePro(LineDrawCallPro drawcall)
{
    vec2 start = drawcall.start;
    vec2 end = drawcall.end;
    color3 color = drawcall.color;
    const GLShader* shader = drawcall.shader;
    const float width = drawcall.width;
    const float alpha = drawcall.alpha;

    // Use default collision shader if no shader is provided
    if (shader == nullptr) {
        shader = Engine::GetShaderManager().GetShader("default_collision");
    }

    vec2 direction = end - start;
    float length = direction.Length(); // Calculate length of the line
    direction = direction.Normalize(); // Normalize direction vector

    float angle = std::atan2(direction.y, direction.x); // Calculate angle of the line

    mat3 model_to_world = mat3::build_translation(start) * mat3::build_rotation(angle)* mat3::build_scale(length);
    
    mat3 WORLD_TO_NDC =
        mat3::build_scale(2.0f / Engine::window_width, 2.0f / Engine::window_height) *
        mat3::build_translation({ -(Engine::window_width / 2), -(Engine::window_height / 2) });
    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;

    shader->Use(); // Use shader
    shader->SendUniform("uModelToNDC", to_span(model_to_ndc)); // Send transformation matrix to shader
    shader->SendUniform("uFillColor", to_span(color)); // Send line color to shader

    glCheck(glLineWidth(width)); // Set line width
    line_model.Use(); // Bind line model
    GLDrawVertices(line_model); // Draw the line

    shader->Use(false); // Unbind shader
    line_model.Use(false); // Unbind line model
    glCheck(glLineWidth(1.0f)); // Set line width
}

mat3 CS230::Render::GetWorldtoNDC()
{
    return Engine::GetGameStateManager().GetGSComponent<CS230::Cam>()->world_to_ndc;
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

void CS230::Render::CreatCircleOutlineModel(int num_segments) // 36
{
    float radius = 0.5f;
    std::vector<vec2> positions;

    for (int i = 0; i <= num_segments; ++i) {
        float theta = 2.0f * 3.1415926f * static_cast<float>(i) / static_cast<float>(num_segments); // 각도 계산
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        positions.emplace_back(x, y);
    }

    const auto positions_byte_size = static_cast<long long>(sizeof(vec2) * positions.size());
    GLsizei buffer_size = static_cast<GLsizei>(positions_byte_size);

    GLVertexBuffer buffer(buffer_size);
    buffer.SetData(std::span(positions));

    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // Layout location for position
    position.stride = sizeof(vec2); // Stride for position
    position.offset = 0; // Offset for position

    circle_outline_model.AddVertexBuffer(std::move(buffer), { position });
    circle_outline_model.SetPrimitivePattern(GLPrimitive::LineLoop); // Line loop
}




//void CS230::Render::DrawBackground(const DrawCall& draw_call)
//{
//    const GLShader* shader = draw_call.shader;
//    shader->Use();
//
//    if (draw_call.texture) {
//        draw_call.texture->UseForSlot(1);
//        shader->SendUniform("uTex2d", 1);
//    }
//    else {
//        throw std::runtime_error("no texture!");
//    }
//
//    vec2 texture_size = (vec2)draw_call.texture->GetSize();
//    mat3 model_to_world = *draw_call.transform * mat3::build_scale(texture_size);
//
//    mat3 WORLD_TO_NDC = GetWorldtoNDC();
//
//    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;
//    shader->SendUniform("uModelToNDC", to_span(model_to_ndc));
//    model.Use();
//    GLDrawIndexed(model);
//
//    model.Use(false);
//    shader->Use(false);
//}