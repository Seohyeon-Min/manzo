#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec2 textureSize;  
uniform bool canCollide;
uniform bool isColliding;

void main() {
    vec2 texelSize = vec2(1.0 / textureSize.x, 1.0 / textureSize.y);
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if (tex_color.a <= 0)
        discard;

    if (isColliding && canCollide) {
        float alpha = tex_color.a;

        // 주변 픽셀 알파 값 가져오기
        float alphaLeft   = texture(uTex2d, vTextureCoordinates - vec2(texelSize.x, 0)).a;
        float alphaRight  = texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0)).a;
        float alphaTop    = texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y)).a;
        float alphaBottom = texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y)).a;

        // 엣지 감지
        float edge = abs(alphaLeft - alphaRight) + abs(alphaTop - alphaBottom);
        edge = smoothstep(0.02, 0.9, edge);  // 감도 조절

        // 가우시안 블러 적용
        // 가우시안 커널을 사용하여 주변 픽셀의 값을 평균내어 블러 처리
        float blurStrength = 0.9;  // 블러 강도 조정
        vec4 blurredColor = vec4(0.0);

        // 주변 9개 픽셀 샘플링 (가우시안 필터 크기 조정 가능)
        float weight[9];
        weight[0] = 1.0 / 16.0; weight[1] = 2.0 / 16.0; weight[2] = 1.0 / 16.0;
        weight[3] = 2.0 / 16.0; weight[4] = 4.0 / 16.0; weight[5] = 2.0 / 16.0;
        weight[6] = 1.0 / 16.0; weight[7] = 2.0 / 16.0; weight[8] = 1.0 / 16.0;

        // 중앙 포함 8방향 주변 샘플링
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, texelSize.y)) * weight[0];  // 왼쪽 상단
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(0.0, texelSize.y)) * weight[1];  // 위
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, texelSize.y)) * weight[2];  // 오른쪽 상단

        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, 0.0)) * weight[3];  // 왼쪽
        blurredColor += tex_color * weight[4];  // 중앙
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0.0)) * weight[5];  // 오른쪽

        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, -texelSize.y)) * weight[6];  // 왼쪽 하단
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(0.0, -texelSize.y)) * weight[7];  // 아래
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, -texelSize.y)) * weight[8];  // 오른쪽 하단

        // 가우시안 블러 강도 적용
        tex_color.rgb = mix(tex_color.rgb, blurredColor.rgb, blurStrength);

        // Glow 효과 적용
        vec3 glowColor = vec3(1.0, 0.8, 0.5);  // 따뜻한 Glow 색상
        float glowIntensity = edge * 1.5;  // 강하게 적용
        tex_color.rgb = mix(tex_color.rgb, glowColor, glowIntensity);
    }

    fFragClr = tex_color;
}
