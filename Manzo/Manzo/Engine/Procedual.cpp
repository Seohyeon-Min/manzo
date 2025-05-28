#include "Procedual.h" 
#include <vector>
#include <cmath> 
#include "../Engine/Lerp.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObject.h"


void ProceduralChain::Initialize(int count, float defaultSize, vec2 start_position) {
    positions.clear();
    circle_size.clear();

    for (int i = 0; i < count; ++i) {
        positions.push_back(start_position);
        circle_size.push_back(defaultSize);
    }
}

void ProceduralChain::Initialize(const std::vector<int>& sizes, vec2 start_position) {
    positions.clear();
    circle_size.clear();

    for (size_t i = 0; i < sizes.size(); ++i) {
        positions.push_back(start_position);
        circle_size.push_back(static_cast<float>(sizes[i]));
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
    for (size_t i = 0; i < positions.size(); ++i) {
        vec2 world_pos = parent_matrix * positions[i];

        CircleDrawCall draw_call = {
            circle_size[i],
            world_pos
        };

        draw_call.settings.do_blending = true;
        draw_call.settings.is_camera_fixed = false;
        draw_call.sorting_layer = layer;

        Engine::GetRender().AddDrawCall(std::make_unique<CircleDrawCall>(draw_call));
    }
}

const std::vector<vec2>& ProceduralChain::GetPositions() const {
    return positions;
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
}