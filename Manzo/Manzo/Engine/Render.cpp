#include <vector>
#include <unordered_map>
#include <filesystem>
#include <memory>

#include "Render.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLVertexArray.h"


// Load a texture and associate it with a model
void Render::LoadTextureAndModel(const std::filesystem::path& texturePath, GLVertexArray* _model) {
    auto texture = std::make_unique<GLTexture>();
    texture->LoadFromFileImage(texturePath);

    auto model = std::make_unique<GLVertexArray>();

    // Store the texture and model in the map
    textureModelMap[texturePath] = _model;
    textures.push_back(std::move(texture));
    models.push_back(std::move(model));
}

// Add a draw call to the corresponding vector based on the phase
void Render::AddDrawCall(const DrawCall& drawCall, const std::string& phase) {
    if (phase == "draw first") {
        draw_first_calls.push_back(drawCall);
    }
    else if (phase == "draw late") {
        draw_late_calls.push_back(drawCall);
    }
    else {
        draw_calls.push_back(drawCall);
    }
}

// Render all draw calls
void Render::RenderAll() {
    // First, render draw_first_calls
    for (const auto& draw_call : draw_first_calls) {
        Draw(draw_call);
    }

    // Then, render draw_calls
    for (const auto& draw_call : draw_calls) {
        Draw(draw_call);
    }

    // Finally, render draw_late_calls
    for (const auto& draw_call : draw_late_calls) {
        Draw(draw_call);
    }

    // Clear the draw call vectors if needed for the next frame
    draw_first_calls.clear();
    draw_calls.clear();
    draw_late_calls.clear();
}


// Internal render method
void Render::Draw(const DrawCall& draw_call) {
    const GLShader* shader = draw_call.texture->GetShader();

    if (draw_call.texture) {
        draw_call.texture->UseForSlot(0); // Bind the texture to the shader
        shader->SendUniform("uTex2d", 0);
    }

    if (draw_call.model) {
        mat3 model_to_world;
        //model_to_world *= model_to_world.build_translation(object.position);
        //model_to_world *= model_to_world.build_rotation(object.rotation);
        //model_to_world *= model_to_world.build_scale(object.scale);

        //mat3 WORLD_TO_NDC;
        //WORLD_TO_NDC *= WORLD_TO_NDC.build_scale(1 / WORLD_SIZE_MAX);

        //const mat3 model_to_ndc = WORLD_TO_NDC * model_to_world;
        //shader->SendUniform("uModelToNDC", to_span(model_to_ndc));
        draw_call.model->Use(); // Bind the model for rendering
        GLDrawIndexed(*draw_call.model);
    }
}
