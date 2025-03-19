#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D uSceneTexture;

void main() {
    vec2 uv = TexCoords;

    const float waveStrength = 0.002;  // 왜곡 강도
    const float waveFrequency = 10.0;  // 주파수
    const float waveSpeed = 1.0;       // 속도 조정

    float wave = sin((iTime * waveSpeed) + (waveFrequency * uv.y)) * waveStrength;
    uv.x += wave;

    uv = clamp(uv, vec2(0.0), vec2(1.0));

    vec3 col = texture(uSceneTexture, uv).rgb;
    FragColor = vec4(col, 1.0);
}