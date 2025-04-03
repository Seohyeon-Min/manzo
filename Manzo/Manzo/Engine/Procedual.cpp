#include "Procedual.h" 
#include <vector>
#include <cmath> 


void ProceduralChain::Initialize(int count, int defaultSize) {
    positions.resize(count, { 0, 0 });
    circle_size.resize(count, defaultSize);
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
            direction = direction / distance;
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

const std::vector<vec2>& ProceduralChain::GetPositions() const {
    return positions;
}
