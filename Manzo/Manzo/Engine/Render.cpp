 #include "Render.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "Engine.h"
#include "GameObject.h"
#include "Camera.h"
#include "to_span.h"

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <span>
#include <array>
#include <iostream>


const float WORLD_SIZE_MAX = (float)std::max(Engine::window_width, Engine::window_height);

Render::Render()
    : postProcessFramebuffer((unsigned int)Engine::window_width, (unsigned int)Engine::window_height) { // 프레임 버퍼 생성
    CreatModel();  // 모델 생성
    CreatLineModel();
    CreateCircleLineModel();
}

// Add a draw call to the corresponding vector based on the draw layer
// Draw calls are grouped into first, normal, and late phases
void Render::AddDrawCall(const DrawCall& drawCall, const DrawLayer& phase) {
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
    else if (phase == DrawLayer::DrawUI)
    {
        draw_ui_calls.push_back(drawCall);
    }
    else if (phase == DrawLayer::DrawDialog)
    {
        draw_ui_calls.push_back(drawCall);
    }
    else {
        draw_calls.push_back(drawCall); // Add to normal phase
    }
}

// Overloaded function to add a line or collision draw call
// depending on whether it is a collision line
void Render::AddDrawCall
(vec2 start, vec2 end, color3 color,float width, float alpha ,const GLShader* shader, bool iscollision) {
    if (iscollision) {
        draw_collision_calls.push_back({ start, end, color, shader }); // Collision line
    }
    else {
        draw_line_calls.push_back({ start, end, color, width, alpha, shader }); // Regular line
    }
}

void Render::AddDrawCall (const CircleDrawCall& drawcall, const DrawLayer& phase) {
    draw_circle_calls.push_back(drawcall); // Regular line
}

// Render all stored draw calls, starting with early phase, normal phase, and then late phase
// Also handles rendering of lines and collision shapes
void Render::RenderAll() {
    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        postProcessFramebuffer.Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    for (const auto& draw_call : draw_background_calls) {
        Draw(draw_call);
    }
    // Draw calls in the early phase
    for (const auto& draw_call : draw_first_calls) {
        Draw(draw_call);
    }

    // Draw normal draw calls
    for (const auto& draw_call : draw_calls) {
        Draw(draw_call);
    }

    for (const auto& draw_call : draw_circle_calls) {
        DrawCircleLine(draw_call);
    }

    // Draw calls in the late phase
    for (const auto& draw_call : draw_late_calls) {
        Draw(draw_call);
    }

    // Draw UI
    for (const auto& draw_call : draw_ui_calls) {
        Draw(draw_call);
    }
    for (const auto& draw_call : draw_dialog_calls) {
        Draw(draw_call);
    }

    // Draw lines
    float line_width = 2.0f;
    for (const auto& draw_call : draw_line_calls) {
        DrawLinePro(draw_call);
    }

    // If collision debug mode is enabled, render collision lines
    if (Engine::GetGameStateManager().GetGSComponent<ShowCollision>() != nullptr &&
        Engine::GetGameStateManager().GetGSComponent<ShowCollision>()->Enabled()) {
        for (const auto& draw_call : draw_collision_calls) {
            DrawLine(draw_call); // Collision lines
        }
    }

    if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        postProcessFramebuffer.Unbind();
        ApplyPostProcessing();
    }

    // Clear draw call vectors for the next frame
    ClearDrawCalls();
}

void Render::ApplyPostProcessing()
{
    auto* bloomShader = Engine::GetShaderManager().GetShader("post_bloom");
    bloomShader->Use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 기본 프레임버퍼로 출력
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postProcessFramebuffer.GetColorAttachment());
    bloomShader->SendUniform("uSceneTexture", 0);
    bloomShader->SendUniform("uThreshold", 0.70f);
    bloomShader->SendUniform("uBlurDirection", 1.0f, 0.0f); // 수평 블러
    bloomShader->SendUniform("uResolution", static_cast<float>(Engine::window_width));
    bloomShader->SendUniform("uBloomIntensity", 1.2f);

    RenderQuad();
    bloomShader->Use(false);
}


// Draw an individual draw call (textured quad)
// Converts world coordinates to normalized device coordinates (NDC)
void Render::Draw(const DrawCall& draw_call) {
    const GLShader* shader = draw_call.shader;
    auto settings = draw_call.settings;
    GLTexture* texture;
    vec2 texture_size;
    vec2 texel_position;
    ivec2 frame_size;

    std::visit([&](auto&& drawable) {
        using T = std::decay_t<decltype(drawable)>;
        if constexpr (std::is_same_v<T, Sprite*>) {
            if (drawable != nullptr) {
                Sprite* sprite = drawable;
                texture = drawable->GetTexture(); // Sprite 텍스처
                texel_position = (vec2)sprite->GetFrameTexel(sprite->GetCurrentFrame()); // 프레임 시작 좌표
                frame_size = sprite->GetFrameSize(); // 프레임 크기

            }
        }
        else if constexpr (std::is_same_v<T, GLTexture*>) {
            if (drawable != nullptr) {
                texture = drawable; // GLTexture 텍스처
                texel_position = { 0, 0 }; // 기본값
                frame_size = { texture->GetSize().x, texture->GetSize().y }; // 전체 텍스처 크기
            }
        }
        }, draw_call.drawable);

    if (shader == nullptr) {
        shader = Engine::GetShaderManager().GetShader("default_collision");
    }
    shader->Use(); // Use the specified shader

    // Ensure the texture is valid, then use it and send it to the shader
    if (texture) {
        //draw_call.texture->SetFiltering(GLTexture::Linear);
        texture->UseForSlot(0);
        shader->SendUniform("uTex2d", 0);

    }
    else {
        throw std::runtime_error("no texture!"); // Error if no texture is assigned
    }

    if (settings.do_blending || settings.glow || settings.modulate_color) {
        glCheck(glEnable(GL_BLEND));
    }
    else {
        glCheck(glDisable(GL_BLEND)); // 블렌딩 비활성화
    }

    if (settings.glow) {
        glCheck(glBlendFunc(GL_ONE, GL_ONE)); // Glow 블렌딩 설정
    }
    else if (settings.do_blending) {
        glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 일반 알파 블렌딩 설정
    }
    else if (settings.modulate_color) {
        glCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
    }

    const float left_u = texel_position.x / float(texture->GetSize().x);
    const float right_u = (texel_position.x + frame_size.x) / float(texture->GetSize().x);
    const float bottom_v = (texel_position.y + frame_size.y) / float(texture->GetSize().y);
    const float top_v = texel_position.y / float(texture->GetSize().y);

    shader->SendUniform("uUV", left_u, top_v, right_u, bottom_v);

    mat3 model_to_world = *draw_call.transform * mat3::build_scale(vec2((float)frame_size.x, (float)frame_size.y)); // 프레임 크기 적용
    mat3 WORLD_TO_NDC = settings.is_UI
        ? mat3::build_scale(2.0f / Engine::window_width, 2.0f / Engine::window_height)
        : GetWorldtoNDC();
    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;

    shader->SendUniform("uModelToNDC", util::to_span(model_to_ndc)); // Send transformation matrix to shader
    if (Engine::GetShaderManager().GetShader("pixelate") == shader) 				
        shader->SendUniform("uPixelSize", 0.008f);

    //if there is a uniform, add
    if (draw_call.SetUniforms) {
        draw_call.SetUniforms(shader);
    }
    
    model.Use(); // Bind the model for drawing
    GLDrawIndexed(model); // Draw the model

    model.Use(false); // Unbind the model
    shader->Use(false); // Unbind the shader
}


void Render::ClearDrawCalls()
{
    draw_first_calls.clear();
    draw_calls.clear();
    draw_late_calls.clear();
    draw_line_calls.clear();
    draw_ui_calls.clear();
    draw_collision_calls.clear();
    draw_circle_calls.clear();
    draw_background_calls.clear();
}


void Render::DrawBackground(const DrawCall& draw_call)
{
    const GLShader* shader = draw_call.shader;
    auto settings = draw_call.settings;

    GLTexture* texture;
    std::visit([&](auto&& drawable) {
        using T = std::decay_t<decltype(drawable)>;
        if constexpr (std::is_same_v<T, Sprite*>) {
            if (drawable != nullptr) {
                texture = drawable->GetTexture(); // sprite
            }
        }
        else if constexpr (std::is_same_v<T, GLTexture*>) {
            if (drawable != nullptr) {
                texture = drawable; // texture
            }
        }
        }, draw_call.drawable);

    shader->Use();

    if (texture) {
        texture->UseForSlot(1);
        shader->SendUniform("uTex2d", 1);
    }
    else {
        throw std::runtime_error("no texture!");
    }

    if (settings.do_blending || settings.glow || settings.modulate_color) {
        glCheck(glEnable(GL_BLEND));
    }
    else {
        glCheck(glDisable(GL_BLEND)); // 블렌딩 비활성화
    }

    if (settings.glow) {
        glCheck(glBlendFunc(GL_ONE, GL_ONE)); // Glow 블렌딩 설정
    }
    else if (settings.do_blending) {
        glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 일반 알파 블렌딩 설정
    }
    else if (settings.modulate_color) {
        glCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
    }


    vec2 texture_size = (vec2)texture->GetSize();
    mat3 model_to_world = *draw_call.transform * mat3::build_scale(texture_size);

    mat3 WORLD_TO_NDC = GetWorldtoNDC();

    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;
    shader->SendUniform("uModelToNDC", util::to_span(model_to_ndc));
    model.Use();
    GLDrawIndexed(model);

    model.Use(false);
    shader->Use(false);
}

// Draw a line between two points for collision or debugging purposes
void Render::DrawLine(LineDrawCall drawcall) {
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
    shader->SendUniform("uModelToNDC", util::to_span(model_to_ndc)); // Send transformation matrix to shader
    shader->SendUniform("uFillColor", util::to_span(color)); // Send line color to shader

    line_model.Use(); // Bind line model
    GLDrawVertices(line_model); // Draw the line

    shader->Use(false); // Unbind shader
    line_model.Use(false); // Unbind line model
}


void Render::DrawLinePro(LineDrawCallPro drawcall)
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
    shader->SendUniform("uModelToNDC", util::to_span(model_to_ndc)); // Send transformation matrix to shader
    shader->SendUniform("uFillColor", util::to_span(color)); // Send line color to shader

    glCheck(glLineWidth(width)); // Set line width
    line_model.Use(); // Bind line model
    GLDrawVertices(line_model); // Draw the line

    shader->Use(false); // Unbind shader
    line_model.Use(false); // Unbind line model
    glCheck(glLineWidth(1.0f)); // Set line width
}

void Render::DrawCircleLine(CircleDrawCall draw_call) {
    float radius = draw_call.radius;
    const GLShader* shader = draw_call.shader;
    color3 color = {255,255,255};
    vec2 position = draw_call.pos;
    auto settings = draw_call.settings;

    if (shader == nullptr) {
        shader = Engine::GetShaderManager().GetShader("default_collision");
    }
    shader->Use();

    if (settings.do_blending || settings.glow || settings.modulate_color) {
        glCheck(glEnable(GL_BLEND)); // 블렌딩 활성화
    }
    else {
        glCheck(glDisable(GL_BLEND)); // 블렌딩 비활성화
    }

    if (settings.glow) {
        glCheck(glBlendFunc(GL_ONE, GL_ONE)); // Glow 블렌딩 설정
    }
    else if (settings.do_blending) {
        glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // 일반 알파 블렌딩 설정
    }

    mat3 model_to_world = mat3::build_translation(position) * mat3::build_scale(radius);

    mat3 WORLD_TO_NDC = settings.is_UI
        ? mat3::build_scale(2.0f / Engine::window_width, 2.0f / Engine::window_height)
        : GetWorldtoNDC();

    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;
    shader->SendUniform("uModelToNDC", util::to_span(model_to_ndc));
    shader->SendUniform("uFillColor", util::to_span(color)); // Send line color to shader

    //if there is a uniform, add
    if (draw_call.SetUniforms) {
        draw_call.SetUniforms(shader);
    }

    circle_line_model.Use();
    GLDrawVertices(circle_line_model);

    circle_line_model.Use(false);
    shader->Use(false);
}

mat3 Render::GetWorldtoNDC()
{
    return Engine::GetGameStateManager().GetGSComponent<Cam>()->world_to_ndc;
}


void Render::SetProjection(const mat3 proj)
{
    projection_matrix = proj;
}

void Render::CreatModel()
{
    float w = 0.5f, h = 0.5f;
    const std::array positions = { vec2{-w, -h}, vec2{w, -h}, vec2{w, h}, vec2{-w, h} };
    constexpr std::array colors = { color3{1, 1, 1}, color3{1, 1, 1}, color3{1, 1, 1}, color3{1, 1, 1} };
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

void Render::CreatLineModel()
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


void Render::CreateCircleLineModel() {
    int segments = 30;
    float radius = 0.5;

    std::vector<vec2> positions;
    positions.reserve(segments);

    float angle_step = 2.0f * 3.1415926535f / static_cast<float>(segments);

    for (int i = 0; i < segments; ++i) {
        float angle = i * angle_step;
        positions.emplace_back(radius * cos(angle), radius * sin(angle));
    }

    const auto positions_byte_size = static_cast<long long>(sizeof(vec2) * positions.size());
    const GLsizei buffer_size = static_cast<GLsizei>(positions_byte_size);
    GLVertexBuffer buffer(buffer_size);
    buffer.SetData(std::span(positions));

    GLAttributeLayout position;
    position.component_type = GLAttributeLayout::Float;
    position.component_dimension = GLAttributeLayout::_2;
    position.normalized = false;
    position.vertex_layout_location = 0; // Layout location for position
    position.stride = sizeof(vec2); // Stride for position
    position.offset = 0; // Offset for position

    circle_line_model.SetVertexCount(segments);
    circle_line_model.AddVertexBuffer(std::move(buffer), { position });
    circle_line_model.SetPrimitivePattern(GLPrimitive::LineLoop);
}

void Render::RenderQuad()
{
    static unsigned int quadVAO = 0;
    static unsigned int quadVBO;
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions    // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}