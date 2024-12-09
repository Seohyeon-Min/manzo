#version 450 core

layout(location = 0) in vec3 vColor;        // RGB 색상
layout(location = 1) in vec2 vTextureCoordinates; // 텍스처 좌표 (사용하지 않음)

layout(location = 0) out vec4 fFragClr;

uniform vec3 uFillColor;
uniform float uAlpha; // 알파값 (0.0 ~ 1.0 범위)

void main() {
    vec4 color = vec4(uFillColor, 1.0); // vColor를 vec4로 변환, 알파값 초기화
    color.a *= uAlpha;              // 알파값 적용
    fFragClr = color;               // 출력 색상 설정
}