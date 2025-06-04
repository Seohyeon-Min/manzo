#include "Procedual.h" 
#include <vector>
#include <cmath> 
#include "../Engine/Lerp.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObject.h"




void ProceduralChain::Initialize(int count, float defaultSize, vec2 start_position) {
    positions.clear();
    circle_size.clear();
    textures.clear();

    for (int i = 0; i < count; ++i) {
        positions.push_back(start_position);
        circle_size.push_back(defaultSize);
        textures.push_back(nullptr);  // 기본 텍스처 없음
    }
}

void ProceduralChain::Initialize(const std::vector<int>& sizes, vec2 start_position, const std::vector<std::string>& texturePaths) {
    
    positions.clear();
    circle_size.clear();
    textures.clear();

    for (size_t i = 0; i < sizes.size(); ++i) {
        positions.push_back(start_position);
        circle_size.push_back(static_cast<float>(sizes[i]));
        if (i < texturePaths.size()) {
            textures.push_back(Engine::GetTextureManager().Load("assets/images/monster.png"));
        }
        else {
            textures.push_back(nullptr);
        }
    }
}

void ProceduralChain::Update(GameObject* headObject, float followSpeed) {
    if (headObject == nullptr) {
        Clear();
        return;
    }

    vec2 head_position = headObject->GetPosition();

    for (int i = 0; i < positions.size(); i++) {
        vec2 direction = (i == 0) ? (head_position - positions[i]) : (positions[i - 1] - positions[i]);
        float distance = direction.Length();
        if (distance > 0.0f) {
            direction = direction / distance;
        }

        vec2 targetPosition = (i == 0)
            ? head_position - direction * circle_size[i]
            : positions[i - 1] - direction * circle_size[i];

        positions[i] = Lerp(positions[i], targetPosition, followSpeed);
    }
}

void ProceduralChain::Draw(const mat3& parent_matrix, DrawLayer layer) {
    GLTexture* tex = Engine::GetTextureManager().Load("assets/images/monster.png");
    mat3 model_matrix = mat3::build_translation({ 4100, -5300 }) * mat3::build_scale(0.1f);
    DrawCall test_draw = {
        tex,
        &model_matrix,
        Engine::GetShaderManager().GetDefaultShader()
    };
    test_draw.settings.do_blending = true;
    test_draw.settings.is_camera_fixed = true;
    Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(test_draw));

    for (size_t i = 0; i < positions.size(); ++i) {
        vec2 world_pos = parent_matrix * positions[i];
        float scale = circle_size[i];

        if (textures[i]) {
            // 각각의 노드에 대해 position과 scale 적용
            mat3 model_matrix = mat3::build_translation(world_pos) *
                mat3::build_scale(scale);

            DrawCall draw_call = {
                textures[i],
                &model_matrix,
                Engine::GetShaderManager().GetDefaultShader()
            };

            draw_call.settings.do_blending = true;
            draw_call.settings.is_camera_fixed = false;
            draw_call.sorting_layer = layer;

            Engine::GetRender().AddDrawCall(std::make_unique<DrawCall>(draw_call));
        }
        else {
            // fallback: circle만 그리는 경우
            CircleDrawCall draw_call2 = {
                scale,
                world_pos
            };

            draw_call2.settings.do_blending = true;
            draw_call2.settings.is_camera_fixed = false;
            draw_call2.sorting_layer = layer;

            Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call2));
        }
    }
}

const vec2 ProceduralChain::GetPositions(int index, const mat3& parent_matrix) const {

    if (index > positions.size()) {
        return { -1,-1 };
    }
    return parent_matrix * positions[index];
}

void ProceduralChain::SetSizeAt(int index, float size) {
    if (index >= 0 && index < circle_size.size()) {
        circle_size[index] = size;
    }
}

void ProceduralChain::SetAllSize(float size) {
    for (auto& s : circle_size) {
        s = size;
    }
}

void ProceduralChain::Clear() {
    positions.clear();
    circle_size.clear();
    textures.clear();
}
