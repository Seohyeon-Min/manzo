#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D uSceneTexture; // 프레임 버퍼 텍스처

void main() {
    vec3 color = texture(uSceneTexture, TexCoords).rgb;
    FragColor = vec4(color, 1.0); // 텍스처 색상을 그대로 출력
}