#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D uSceneTexture;
uniform float uThreshold; // 밝기 임계값

void main() {
    vec3 color = texture(uSceneTexture, TexCoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722)); // 밝기 계산 (Luminance)
    if (brightness > uThreshold) {
        FragColor = vec4(color, 1.0); // 밝은 영역 유지
    } else {
        FragColor = vec4(0.0); // 어두운 영역 제거
    }
}