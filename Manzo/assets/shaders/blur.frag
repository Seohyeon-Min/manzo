#version 450 core

// 입력 변수
layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

// 유니폼 변수
uniform sampler2D uTexture;   // 오브젝트의 텍스처
uniform vec2 uResolution;     // 화면 해상도
uniform vec2 uBlurDirection;  // 블러 방향 (가로 또는 세로)

// 출력 변수
out vec4 FragColor;

// 가우시안 커널 가중치
const float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
    vec2 texelSize = 1.0 / uResolution; // 텍셀 크기 계산
    vec3 color = texture(uTexture, vTextureCoordinates).rgb * weights[0]; // 중심 픽셀

    // 블러링 계산
    for (int i = 1; i < 5; ++i) {
        vec2 offset = uBlurDirection * float(i) * texelSize; // 텍셀 오프셋
        color += texture(uTexture, vTextureCoordinates + offset).rgb * weights[i];
        color += texture(uTexture, vTextureCoordinates - offset).rgb * weights[i];
    }

    FragColor = vec4(color, 1.0);
}