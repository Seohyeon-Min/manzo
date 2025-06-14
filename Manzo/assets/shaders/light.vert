#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 vWorldPos;

uniform mat3 uModelToWorld;
uniform mat3 uModelToNDC;

void main() {
    vec3 worldPos = uModelToWorld * vec3(aPos, 1.0);
    vWorldPos = worldPos.xy;

    vec3 ndcPos = uModelToNDC * vec3(aPos, 1.0);
    gl_Position = vec4(ndcPos.xy, 0.0, 1.0);
}
