#version 450 core

layout(location = 1) in vec2 vTextureCoordinates;
layout(location = 0) out vec4 fragColor;

uniform float uTime;
uniform float uStartTime;
uniform float uDelay;
uniform vec2 uResolution;      // 화면 해상도

vec4 transitionCircle(vec2 uv, float progression) {
    uv -= vec2(0.5); // 중심 기준 이동
    uv.x *= uResolution.x / uResolution.y; // 종횡비 보정

    float additionalLength = sin(uv.x * 50.0) + sin(uv.y * 50.0);
    additionalLength *= 0.012;

    float radius = length(uv) + additionalLength;

    float scaledProgression = progression * 1.5; // 덮는 범위 확대

    if (radius < scaledProgression) {
        return vec4(0.0, 0.0, 0.0, 1.0); // 검정, 불투명
    } else {
        return vec4(0.0, 0.0, 0.0, 0.0); // 완전 투명
    }
}

void main() {
    vec2 uv = vTextureCoordinates;

    float t = max(0.0, (uTime - uStartTime - uDelay) / 8.0);
    float linear = clamp(t, 0.0, 1.0);
    float progression = 1.0 - pow(1.0 - linear, 5.0); // ease-out cubic
    fragColor = transitionCircle(uv, progression);

    // 투명한 부분은 그리지 않음
    if (fragColor.a == 0.0)
        discard;
}
