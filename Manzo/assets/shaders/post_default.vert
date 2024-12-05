#version 450 core

layout(location = 0) in vec2 aPos;      // Quad의 정점 좌표
layout(location = 1) in vec2 aTexCoords; // Quad의 텍스처 좌표

out vec2 TexCoords; // 프래그먼트 셰이더로 전달할 텍스처 좌표

void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0); // NDC 좌표로 변환
}