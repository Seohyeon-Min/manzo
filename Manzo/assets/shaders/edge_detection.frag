#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform bool canCollideWithMouse;
uniform vec2 texelSize;  // (1.0 / texture width, 1.0 / texture height)

void main() {
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if (tex_color.a <= 0)
        discard;

    float alpha = tex_color.a;

    // 주변 픽셀 알파 값 가져오기
    float alphaLeft   = texture(uTex2d, vTextureCoordinates - vec2(texelSize.x, 0)).a;
    float alphaRight  = texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0)).a;
    float alphaTop    = texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y)).a;
    float alphaBottom = texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y)).a;

    // 엣지 감지
    float edge = abs(alphaLeft - alphaRight) + abs(alphaTop - alphaBottom);
    edge = smoothstep(0.02, 0.1, edge);  // 감도 조절


        vec3 glowColor = vec3(1.0, 0.8, 0.5);  // 따뜻한 Glow 색상
        float glowIntensity = edge * 1.5;  // 강하게 적용
        tex_color.rgb = mix(tex_color.rgb, glowColor, glowIntensity);
    

    fFragClr = tex_color;
}
