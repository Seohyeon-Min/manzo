#version 450 core

out vec4 FragColor; // 출력 색상

in vec2 TexCoords; // 텍스처 좌표

uniform sampler2D uSceneTexture; // 장면 텍스처

void main() {
    vec3 color = texture(uSceneTexture, TexCoords).rgb; // 텍스처에서 색상 샘플링
    FragColor = vec4(color, 1.0); // 텍스처 색상을 그대로 출력
}