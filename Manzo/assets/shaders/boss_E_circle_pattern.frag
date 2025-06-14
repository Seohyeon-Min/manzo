#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 outColor;

uniform float uRadius;         // 정규화 반지름 (0.0 ~ 1.0)
uniform vec2 uResolution;      // 예: vec2(1920.0, 1080.0)

void main()
{
    vec2 uv = gl_FragCoord.xy / uResolution;
    vec2 center = vec2(0.5, 0.5);

    // 보정: 화면 비율을 적용
    uv.x *= uResolution.x / uResolution.y;
    center.x *= uResolution.x / uResolution.y;

    float dist = distance(uv, center);

    if (dist > uRadius)
        outColor = vec4(1.0, 0.0, 0.0, 1.0);  // 빨간색
    else
        outColor = vec4(0.0, 0.0, 0.0, 0.0);  // 투명
}
