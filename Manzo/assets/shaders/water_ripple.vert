#version 450 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vTextureCoordinates;

uniform mat3 uModelToNDC;
uniform vec4 uUV;

// 추가된 uniform: 시간값
uniform float uTime;

void main()
{
    // 물결 offset 계산
    float waveStrength = 0.02;  // 찰랑거리는 세기
    float waveFrequency = 10.0; // 얼마나 자주 반복되는지
    float waveSpeed = 2.0;      // 시간에 따른 속도

    // y방향으로 살짝 흔들기 (x에 따라 y가 흔들림)
    float waveOffset = sin(aVertexPosition.x * waveFrequency + uTime * waveSpeed) * waveStrength;

    vec2 displacedPosition = aVertexPosition + vec2(0.0, waveOffset);

    gl_Position = vec4(vec2(uModelToNDC * vec3(displacedPosition, 1.0)), 0.0, 1.0);
    vColor = aVertexColor;
    vTextureCoordinates = vec2(
        mix(uUV.x, uUV.z, aTexCoord.x),
        mix(uUV.y, uUV.w, aTexCoord.y)
    );
}