#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 vWorldPos;

uniform vec2 uScreenSize;
uniform mat3 uModelToWorld;
uniform mat3 uModelToNDC;
uniform mat3 uScreenToWorld;

void main() {
vec3 screenPos = vec3(aPos * 0.5 + 0.5, 1.0); // [0, 1] 범위로 변환
screenPos.xy *= uScreenSize;
gl_Position = vec4(aPos, 0.0, 1.0);
vec3 worldPos = uScreenToWorld * screenPos;
vWorldPos = worldPos.xy;
}
