#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 outColor;

uniform sampler2D uTexture;
uniform float uTime;
uniform bool uX;
uniform bool uY;
uniform float waveStrength;
uniform float frequency;
uniform float speed;

void main()
{
    vec2 uv = vTextureCoordinates;

    float wave1 = sin(uv.x * frequency + uTime * speed) * waveStrength;
    float wave2 = cos(uv.y * frequency * 0.7 + uTime * speed * 1.2) * waveStrength * 0.7;
    float wave3 = sin((uv.x + uv.y) * frequency * 0.5 + uTime * speed * 1.5) * waveStrength * 0.5;
    float wave4 = cos((uv.x - uv.y) * frequency * 0.9 - uTime * speed * 0.8) * waveStrength * 0.3;

    if (uX) uv.x += wave1;
    if (uY) uv.y += wave3; 

    vec4 texColor = texture(uTexture, uv);
    outColor = texColor * vec4(vColor, 1.0);
}
