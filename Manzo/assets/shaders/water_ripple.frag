#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 outColor;

uniform sampler2D uTexture;
uniform float uTime;

void main()
{
    // 기본 왜곡 강도 및 주파수/속도 설정
    float waveStrength = 0.014;
    float frequency = 48.0;
    float speed = 1.0;

    vec2 uv = vTextureCoordinates;

    // 여러 파동을 중첩하여 복잡한 왜곡 효과 생성
    float wave1 = sin(uv.x * frequency + uTime * speed) * waveStrength;
    float wave2 = cos(uv.y * frequency * 0.7 + uTime * speed * 1.2) * waveStrength * 0.7;
    float wave3 = sin((uv.x + uv.y) * frequency * 0.5 + uTime * speed * 1.5) * waveStrength * 0.5;
    float wave4 = cos((uv.x - uv.y) * frequency * 0.9 - uTime * speed * 0.8) * waveStrength * 0.3;

    // 각각의 파동을 합산하여 UV 좌표를 왜곡
    uv.x += wave1 + wave2;
    uv.y += wave3 + wave4;

    vec4 texColor = texture(uTexture, uv);
    outColor = texColor * vec4(vColor, 1.0);
}
