#version 450 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D uSceneTexture;  // 원본 텍스처
uniform float uThreshold;         // 밝기 임계값
uniform vec2 uBlurDirection;      // 블러 방향
uniform float uResolution;        // 텍스처 해상도
uniform float uBloomIntensity;    // 블룸 강도

void main() {
    vec3 sceneColor = texture(uSceneTexture, TexCoords).rgb;

    vec3 expandedBrightColor = vec3(0.0); // 초기화
    int radius = 5;                       // 확장 반경 (5픽셀)
    float weightSum = 0.0;                 // 가중치 합산 (정규화)
    float stepSize = 1.0 / uResolution;   // 텍스처 좌표 스텝 크기

    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            vec2 offset = vec2(x, y) * stepSize;
            float distance = length(vec2(x, y));    // 현재 픽셀까지의 거리
            if (distance <= float(radius)) {        // 반경 내에서만 처리
                float weight = 1.0 - (distance / float(radius));    // 거리 기반 가중치
                vec3 neighborColor = texture(uSceneTexture, TexCoords + offset).rgb;
                float neighborBrightness = dot(neighborColor, vec3(0.3, 0.59, 0.11)) * 0.7;                             // maybe brightness?
                float neighborSoftBrightness = smoothstep(uThreshold - 0.2, uThreshold + 0.04, neighborBrightness);
                vec3 neighborBrightColor = neighborSoftBrightness * neighborColor;
                expandedBrightColor += neighborBrightColor * weight;
                weightSum += weight;
            }
        }
    }

    if (weightSum > 0.0) {
        expandedBrightColor /= weightSum; // 가중치 정규화
    }

    vec3 result = sceneColor + expandedBrightColor * uBloomIntensity;
    FragColor = vec4(result, 1.0); // 최종 출력
}
