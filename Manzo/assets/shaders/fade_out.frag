#version 450 core

layout(location = 1) in vec2 vTextureCoordinates;
layout(location = 0) out vec4 fragColor;

uniform sampler2D uTex2d;
uniform float uTime;           // 현재 시간
uniform float uStartDelay;     // 몇 초 후 페이드 아웃 시작
uniform float uFadeDuration;   // 페이드 아웃 지속 시간

void main()
{
    vec4 texColor = texture(uTex2d, vTextureCoordinates);

    float fadeRatio = clamp((uTime - uStartDelay) / uFadeDuration, 0.0, 1.0);

    float alpha = mix(1.0, 0.0, fadeRatio);
    fragColor = vec4(texColor.rgb, texColor.a * alpha);

    if(texColor.a <= 0)
        discard;
}
