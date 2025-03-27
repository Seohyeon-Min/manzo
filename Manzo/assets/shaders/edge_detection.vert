#version 450 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vTextureCoordinates;

uniform mat3 uModelToNDC;
uniform vec4 uUV;

void main()
{
    gl_Position = vec4(vec2(uModelToNDC * vec3(aVertexPosition, 1.f)), 0.0, 1.0);
    vColor = aVertexColor;
    vTextureCoordinates = vec2(
        mix(uUV.x, uUV.z, aTexCoord.x),
        mix(uUV.y, uUV.w, aTexCoord.y)
    );
}