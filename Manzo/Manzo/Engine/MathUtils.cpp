#include "MathUtils.h"

vec2 ClosestPoint(vec2 point, vec2 a, vec2 b) {
	vec2 ab = b - a;
	vec2 ap = point - a;

	float t = dot(ap, ab) / ab.LengthSquared();
	t = std::clamp(t, 0.0f, 1.0f);

	return a + ab * t;
}

vec2 Lerp(const vec2& start, const vec2& end, float t) {
	return start + t * (end - start);
}

float Lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

mat3 Inverse(mat3& m)
{
    float det =
        m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
        m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
        m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    if (det == 0.0f)
        return mat3(); // �Ǵ� assert(false), ���� ��

    float invDet = 1.0f / det;

    mat3 result;

    result[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    result[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * invDet;
    result[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

    result[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * invDet;
    result[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    result[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * invDet;

    result[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    result[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * invDet;
    result[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    return result;
}
