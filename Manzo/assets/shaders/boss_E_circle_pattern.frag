#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 outColor;

uniform float uRadius;         // 중심 투명 영역의 반지름 (고정)
uniform vec2 uResolution;
uniform float uTime;
uniform float uStartTime;

void main()
{
    vec2 uv = gl_FragCoord.xy / uResolution;
    vec2 center = vec2(0.5, 0.5);

    // 종횡비 보정
    float aspect = uResolution.x / uResolution.y;
    uv.x *= aspect;
    center.x *= aspect;

    float dist = distance(uv, center);

    // 시간 기반 진행도 (0 → 1)
    float t = max(0.0, (uTime - uStartTime) / 5.0); // 5초 트랜지션
    float linear = clamp(t, 0.0, 1.0);
    float progression = 1.0 - pow(1.0 - linear, 7.0); // ease-out

    // 🔥 반지름이 점점 줄어들게 설정
    float animatedRadius = mix(1.5, uRadius, progression);

    // 그라데이션 알파 계산 (최대 0.5로 제한)
    float rawAlpha = smoothstep(uRadius, animatedRadius, dist);
    float alpha = rawAlpha * 0.2;

    vec4 red = vec4(0.7882, 0.0549, 0.4196, 0.2); 

    outColor = vec4(red.rgb, alpha);
    if (outColor.a == 0.0)
        discard;
}
