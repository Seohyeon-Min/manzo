#include "Procedual.h" 
#include <vector>
#include <cmath> 
#include "../Engine/Lerp.h"

void Initialize(int count, float defaultSize, vec2 start_position);

void ProceduralChain::Initialize(int count, float defaultSize, vec2 start_position) {
    positions.clear();
    circle_size.clear();

    for (int i = 0; i < count; ++i) {
        positions.push_back(start_position);
        circle_size.push_back(defaultSize);
    }
}

void ProceduralChain::Update(vec2 head_position, float followSpeed) {
    for (int i = 0; i < positions.size(); i++) {
        vec2 direction;
        if (i == 0) {
            direction = head_position - positions[i];
        }
        else {
            direction = positions[i - 1] - positions[i];
        }

        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
        if (distance > 0.0f) {
            direction = { direction / distance };
        }

        vec2 targetPosition;
        if (i == 0) {
            targetPosition = head_position - direction * circle_size[i];
        }
        else {
            targetPosition = positions[i - 1] - direction * circle_size[i];
        }

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