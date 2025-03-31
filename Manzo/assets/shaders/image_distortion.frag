#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D uTex2d;

void main() {
    vec2 uv = vTextureCoordinates;

    const float waveStrength = 0.006;  // 왜곡 강도
    const float waveFrequency = 20.0;  // 주파수
    const float waveSpeed = 1.0;       // 속도 조정

    float wave1 = sin(iTime * waveSpeed + waveFrequency * uv.y) * waveStrength;
    float wave2 = sin(iTime * waveSpeed * 1.2 + waveFrequency * 0.8 * uv.x) * (waveStrength * 0.5);
    uv.x += wave1;
    uv.y += wave2;

    uv = clamp(uv, vec2(0.0), vec2(1.0));

    vec4 col = texture(uTex2d, uv);
    if(col.a <= 0)
        discard;

    fFragClr = col;
}