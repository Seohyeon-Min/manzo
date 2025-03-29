#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec2 textureSize;

void main() {
    vec2 texelSize = vec2(1.0/textureSize.x, 1.0/textureSize.y);
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if (tex_color.a <= 0)
        discard;

    // Glow 감지 범위 확장 (멀티 샘플링)
    float edge = 0.0;
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(texelSize.x * 1.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(texelSize.x * 1.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y * 1.0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y * 1.0)).a - tex_color.a);

    // 더 넓은 영역 샘플링
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(texelSize.x * 2.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(texelSize.x * 2.0, 0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y * 2.0)).a - tex_color.a);
    edge += abs(texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y * 2.0)).a - tex_color.a);

    // 부드럽게 Glow 적용
    edge = smoothstep(0.01, 0.1, edge);  // 두께 증가

    // 더 강한 Glow 효과 적용
    if (edge > 0.0) {
        vec3 glowColor = vec3(1.0, 0., 0.);  // 따뜻한 Glow 색상
        float glowIntensity = pow(edge, 1.5) * 2.5;  // 밝기 증가
        tex_color.rgb += glowIntensity * glowColor;
    }

    fFragClr = tex_color;
}
