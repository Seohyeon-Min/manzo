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

Render::Render(){
    CreatModel();
    CreatLineModel();
    CreateCircleLineModel();
    postProcessFramebuffer[0].Creat((unsigned int)Engine::window_width, (unsigned int)Engine::window_height);
    postProcessFramebuffer[1].Creat((unsigned int)Engine::window_width, (unsigned int)Engine::window_height);
    //all_draw_calls.reserve(1000);

}

void Render::AddDrawCall(std::unique_ptr<BaseDrawCall> drawCall) {
    //all_draw_calls.push_back(std::move(drawCall));
    int layer = static_cast<int>(drawCall->sorting_layer);

    if (layer >= all_draw_calls.size()) {
        all_draw_calls.resize(layer + 1);
    }

    all_draw_calls[layer].push_back(std::move(drawCall));
}

void Render::AddBackgroundDrawCall(const DrawCall& drawCall) {
    draw_background_calls.push_back(drawCall);
}

void Render::AddDrawCall
(vec2 start, vec2 end, color3 color,float width, float alpha ,const GLShader* shader) {
    LineDrawCall line;
    line.start = start; line.end = end; line.color = color;
    draw_collision_calls.push_back(line); // Collision line
}

// Render all stored draw calls, starting with early phase, normal phase, and then late phase
// Also handles rendering of lines and collision shapes
void Render::RenderAll() {

    postProcessFramebuffer[0].Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& draw_call : draw_background_calls) {
        DrawBackground(draw_call);
    }

    if (Engine::GetGameStateManager().GetStateName() == "Title") {
        postProcessFramebuffer[1].Unbind();
        ApplyPostProcessing(0);
        postProcessFramebuffer[0].Bind();
    }

    // Render all draw calls
    for (auto& layer : all_draw_calls) {

        for (auto& draw_call : layer) {
            if (auto* call = dynamic_cast<DrawCall*>(draw_call.get())) {
                Draw(*call);
            }
            else if (auto* call = dynamic_cast<LineDrawCall*>(draw_call.get())) {
                DrawLine(*call);
            }
            else if (auto* call = dynamic_cast<LineDrawCallPro*>(draw_call.get())) {
                DrawLinePro(*call);
            }
            else if (auto* call = dynamic_cast<CircleDrawCall*>(draw_call.get())) {
                DrawCircleLine(*call);
            }
        }
    }

    // If collision debug mode is enabled, render collision lines
    if (Engine::GetGameStateManager().GetGSComponent<ShowCollision>() != nullptr &&
        Engine::GetGameStateManager().GetGSComponent<ShowCollision>()->Enabled()) {
        for (const auto& draw_call : draw_collision_calls) {
            DrawLine(draw_call); // Collision lines
        }
    }


    if (Engine::GetGameStateManager().GetStateName() != "Title") {
        postProcessFramebuffer[1].Unbind();
        ApplyPostProcessing();
    }
    else if (Engine::GetGameStateManager().GetStateName() == "Title") {
        postProcessFramebuffer[1].Unbind();
        ApplyPostProcessing(1);
    }

    // Clear draw call vectors for the next frame
    ClearDrawCalls();
}

void Render::ApplyPostProcessing(bool is_title)
{
    bool horizontal = true, first_iteration = true;


    if (Engine::GetGameStateManager().GetStateName() == "Mode2") {
        int num_passes = 1; // Number of post process
        for (int i = 0; i < num_passes; i++) {
            postProcessFramebuffer[horizontal].Bind();
            glClear(GL_COLOR_BUFFER_BIT);

            GLShader* shader = Engine::GetShaderManager().GetShader("post_bloom");

            switch (i) {
            case 0: // Bloom
                shader = Engine::GetShaderManager().GetShader("post_bloom");
                break;
            }
            shader->Use();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, postProcessFramebuffer[!horizontal].GetColorAttachment());

            double currentTime = Engine::GetAudioManager().GetCurrentMusicTime("background1");

            switch (i) {
            case 0: // Bloom
                shader->SendUniform("uSceneTexture", 0);
                shader->SendUniform("uThreshold", 0.71f);
                shader->SendUniform("uBlurDirection", 1.0f, 1.0f);
                shader->SendUniform("uResolution", static_cast<float>(Engine::window_width));
                shader->SendUniform("uBloomIntensity", 1.1f);
                break;
            }

            RenderQuad();
            shader->Use(false);

            horizontal = !horizontal;
            first_iteration = false;
        }
    }
    else if (Engine::GetGameStateManager().GetStateName() == "Mode1") {
        int num_passes = 3; // Number of post process

        for (int i = 0; i < num_passes; i++) {
            postProcessFramebuffer[horizontal].Bind();
            glClear(GL_COLOR_BUFFER_BIT);

            GLShader* shader = Engine::GetShaderManager().GetShader("post_underwater_distortion");

            switch (i) {
            case 0: // Distortion
                shader = Engine::GetShaderManager().GetShader("post_underwater_distortion");
                break;
            case 1: // Bloom
                shader = Engine::GetShaderManager().GetShader("post_bloom"); 
                break;
            case 2: // God Ray
                shader = Engine::GetShaderManager().GetShader("under_water_god_ray");
                break;
            }
            shader->Use();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, postProcessFramebuffer[!horizontal].GetColorAttachment());

            double currentTime = Engine::GetAudioManager().GetCurrentMusicTime("background1");

            switch (i) {
            case 0: // Distortion
                shader->SendUniform("uSceneTexture", 0);
                shader->SendUniform("iResolution", Engine::window_width, Engine::window_height);
                shader->SendUniform("iTime", float(currentTime));
            case 1: // Bloom
                shader->SendUniform("uSceneTexture", 0);
                shader->SendUniform("uThreshold", 0.8f);
                shader->SendUniform("uBlurDirection", 1.0f, 1.0f);
                shader->SendUniform("uResolution", static_cast<float>(Engine::window_width));
                shader->SendUniform("uBloomIntensity", 1.1f);
                break;
            case 2: // God Ray
                shader->SendUniform("uSceneTexture", 0);
                shader->SendUniform("iResolution", Engine::window_width, Engine::window_height);
                shader->SendUniform("iTime", float(currentTime));
                break;
            }

            RenderQuad();
            shader->Use(false);

            horizontal = !horizontal;
            first_iteration = false;
        }

    }
    else if (Engine::GetGameStateManager().GetStateName() == "Title" && !is_title) {
        int num_passes = 2; // Number of post process
        for (int i = 0; i < num_passes; i++) {
            postProcessFramebuffer[horizontal].Bind();
            glClear(GL_COLOR_BUFFER_BIT);

            GLShader* shader = Engine::GetShaderManager().GetShader("post_bloom");

            switch (i) {
            case 0: // Gra
                shader = Engine::GetShaderManager().GetShader("title_gradation");
                break;
            case 1: // Ripple
                shader = Engine::GetShaderManager().GetShader("title_ripple");
                break;
            }
            shader->Use();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, postProcessFramebuffer[!horizontal].GetColorAttachment());

            double currentTime = Engine::GetAudioManager().GetCurrentMusicTime("title_bgm");

            switch (i) {
            case 0: // Gra
                shader->SendUniform("iResolution", Engine::window_width, Engine::window_height);
                shader->SendUniform("iTime", float(currentTime));
                break;
            case 1: // Ripple
                shader->SendUniform("iResolution", Engine::window_width, Engine::window_height);
                shader->SendUniform("iTime", float(currentTime));
                break;
            }

            RenderQuad();
            shader->Use(false);

            horizontal = !horizontal;
            first_iteration = false;
        }
    }
    else if (Engine::GetGameStateManager().GetStateName() == "Title" && is_title) {
        int num_passes = 1; // Number of post process
        for (int i = 0; i < num_passes; i++) {
            postProcessFramebuffer[horizontal].Bind();
            glClear(GL_COLOR_BUFFER_BIT);

            GLShader* shader = Engine::GetShaderManager().GetShader("post_bloom");

            switch (i) {
            case 0: // Bloom
                shader = Engine::GetShaderManager().GetShader("post_bloom");
                break;
            case 1: // Wave
                shader = Engine::GetShaderManager().GetShader("post_water_wave");
                break;
            }
            shader->Use();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, postProcessFramebuffer[!horizontal].GetColorAttachment());

            double currentTime = Engine::GetAudioManager().GetCurrentMusicTime("title_bgm");

            switch (i) {
            case 0: // Bloom
                shader->SendUniform("uSceneTexture", 0);
                shader->SendUniform("uThreshold", 0.81f);
                shader->SendUniform("uBlurDirection", 1.0f, 1.0f);
                shader->SendUniform("uResolution", static_cast<float>(Engine::window_width));
                shader->SendUniform("uBloomIntensity", 0.1f);
                break;
            case 1: // wave
                shader->SendUniform("iResolution", Engine::window_width, Engine::window_height);
                shader->SendUniform("iTime", float(currentTime));
                shader->SendUniform("uSceneTexture", 0);
                shader->SendUniform("iMouse", Engine::GetInput().GetMousePos().mouseWorldSpaceX, Engine::GetInput().GetMousePos().mouseWorldSpaceY);
                break;
            }

            RenderQuad();
            shader->Use(false);

            horizontal = !horizontal;
            first_iteration = false;
        }
    }

    // Post pass-through
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto* finalShader = Engine::GetShaderManager().GetShader("post_default");
    finalShader->Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postProcessFramebuffer[!horizontal].GetColorAttachment()); // 마지막 패스 결과를 사용

    RenderQuad();
    finalShader->Use(false);
}


void Render::ClearDrawCalls()
{
    all_draw_calls.clear();
    draw_collision_calls.clear();
    draw_background_calls.clear();
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
                texture = drawable->GetTexture();
                texel_position = (vec2)sprite->GetFrameTexel(sprite->GetCurrentFrame());
                frame_size = sprite->GetFrameSize();

            }
        }
        else if constexpr (std::is_same_v<T, GLTexture*>) {
            if (drawable != nullptr) {
                texture = drawable; 
                texel_position = { 0, 0 };
                frame_size = { texture->GetSize().x, texture->GetSize().y };
            }
        }
        }, draw_call.drawable);


    mat3 model_to_world = *draw_call.transform * mat3::build_scale(vec2((float)frame_size.x, (float)frame_size.y));
    mat3 WORLD_TO_NDC = settings.is_camera_fixed
        ? mat3::build_scale(2.0f / Engine::window_width, 2.0f / Engine::window_height)
        : GetWorldtoNDC();
    const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;


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


    //glEnable(GL_SCISSOR_TEST);
    //glScissor(camera_left, camera_bottom, camera_width, camera_height);

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


void Render::DrawBackground(const DrawCall& draw_call) {
    const GLShader* shader = draw_call.shader;
    auto settings = draw_call.settings;
    GLTexture* texture = nullptr;
    vec2 texel_position;
    ivec2 frame_size;

    // Drawable 객체 처리
    std::visit([&](auto&& drawable) {
        using T = std::decay_t<decltype(drawable)>;
        if constexpr (std::is_same_v<T, Sprite*>) {
            if (drawable != nullptr) {
                Sprite* sprite = drawable;
                texture = sprite->GetTexture();
                texel_position = (vec2)sprite->GetFrameTexel(sprite->GetCurrentFrame());
                frame_size = sprite->GetFrameSize();
            }
        }
        else if constexpr (std::is_same_v<T, GLTexture*>) {
            if (drawable != nullptr) {
                texture = drawable;
                texel_position = { 0, 0 };
                frame_size = { texture->GetSize().x, texture->GetSize().y };
            }
        }
        }, draw_call.drawable);

    if (!texture) {
        throw std::runtime_error("No texture provided for background drawing.");
    }

    vec2 texture_size = (vec2)texture->GetSize();

    // 모델 -> 월드 변환
    mat3 model_to_world = *draw_call.transform * mat3::build_scale(texture_size);

    // 월드 -> NDC 변환
    mat3 WORLD_TO_NDC = GetWorldtoNDC();
    mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;

    // 렌더링
    shader->Use();
    texture->UseForSlot(0);
    shader->SendUniform("uTex2d", 0);

    // 블렌딩 설정
    if (settings.do_blending || settings.glow || settings.modulate_color) {
        glCheck(glEnable(GL_BLEND));
        if (settings.glow) {
            glCheck(glBlendFunc(GL_ONE, GL_ONE));
        }
        else if (settings.do_blending) {
            glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }
        else if (settings.modulate_color) {
            glCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
        }
    }
    else {
        glCheck(glDisable(GL_BLEND));
    }

    shader->SendUniform("uModelToNDC", util::to_span(model_to_ndc));

    // 모델 렌더링
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
    auto settings = drawcall.settings;

    // Use default collision shader if no shader is provided
    if (shader == nullptr) {
        shader = Engine::GetShaderManager().GetShader("default_collision");
    }

    vec2 direction = end - start;
    float length = direction.Length(); // Calculate length of the line
    direction = direction.Normalize(); // Normalize direction vector

    float angle = std::atan2(direction.y, direction.x); // Calculate angle of the line

    mat3 model_to_world = mat3::build_translation(start) * mat3::build_rotation(angle)* mat3::build_scale(length);
    
    mat3 WORLD_TO_NDC = settings.is_camera_fixed
        ? mat3::build_scale(2.0f / Engine::window_width, 2.0f / Engine::window_height)
        : GetWorldtoNDC();
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

    mat3 WORLD_TO_NDC = settings.is_camera_fixed
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
    float radius = 1;

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