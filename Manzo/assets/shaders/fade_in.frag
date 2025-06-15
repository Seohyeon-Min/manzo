#version 450 core

layout(location = 1) in vec2 vTextureCoordinates;
layout(location = 0) out vec4 outColor;

uniform sampler2D uTex2d;
uniform vec3 uStartColor;
uniform float uTime;          // 현재 전역 시간
uniform float uStartTime;     // 이 오브젝트가 등장한 시간
uniform float uFadeDuration;  // 페이드인에 걸리는 시간 (ex. 0.5)

void main()
{
    vec4 texColor = texture(uTex2d, vTextureCoordinates);
    vec3 targetColor = texColor.rgb;

    float localTime = uTime - uStartTime;
    float fadeFactor = clamp(localTime / uFadeDuration, 0.0, 1.0);

    vec3 finalColor = mix(uStartColor, texColor.rgb, fadeFactor);
    outColor = vec4(finalColor, texColor.a);

    if(texColor.a <= 0)
        discard;
}
