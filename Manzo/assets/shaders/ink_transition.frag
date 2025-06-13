#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;
layout(location = 0) out vec4 fragColor;

uniform sampler2D uTex2d;
uniform float uTime;           // 셰이더가 시작된 기준 시간
uniform float uStartTime;      // 트랜지션 기준 시작 시간
uniform vec2 uResolution;      // 화면 해상도

// 트랜지션 원형 효과 함수
vec4 transitionCircle(vec2 uv, vec4 color, float progression) {
    uv -= vec2(0.5);                       // 중심 기준으로 이동
    uv.x *= uResolution.x / uResolution.y; // 종횡비 보정

    float additionalLength = sin(uv.x * 50.0) + sin(uv.y * 50.0);
    additionalLength *= 0.012;

    if (length(uv) + additionalLength < progression) {
        color = vec4(0.0, 0.0, 0.0, 1.0); // 검정으로 덮기
    }

    return color;
}

void main()
{
    vec2 uv = vTextureCoordinates;
    vec4 texColor = texture(uTex2d, uv);

    // 진행도 계산: 트랜지션 시작 후부터 진행도 증가 (0 → 1)
    float localTime = max(0.0, uTime - uStartTime); // 지연 고려
    float progression = clamp(localTime / 3.0, 0.0, 1.0); // 3초 동안 진행

    fragColor = transitionCircle(uv, texColor, progression);
    
    // 불투명도 검사
    if (fragColor.a <= 0.0)
        discard;
}
