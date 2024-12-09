#include <vector>
#include <vec2.h>

struct AABB {
    vec2 center;
    vec2 halfSize;

    bool contains(const vec2& point) const {
        return point.x >= center.x - halfSize.x && point.x <= center.x + halfSize.x &&
            point.y >= center.y - halfSize.y && point.y <= center.y + halfSize.y;
    }

    bool intersects(const AABB& range) const {
        return !(range.center.x - range.halfSize.x > center.x + halfSize.x ||
            range.center.x + range.halfSize.x < center.x - halfSize.x ||
            range.center.y - range.halfSize.y > center.y + halfSize.y ||
            range.center.y + range.halfSize.y < center.y - halfSize.y);
    }
};