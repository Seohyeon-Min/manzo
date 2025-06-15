#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uSceneTexture;
uniform float uTime;
uniform vec2 uResolution;

// Parameters
uniform float uFreq = 8.0;
uniform float uPeriod = 8.0;
uniform float uSpeed = 3.0;
uniform float uFade = 3.0;
uniform float uDisplacement = 0.2;
void main() {
    vec2 resolution = uResolution;

    // -------- sceneColor 샘플링용 종횡비 보정 --------
    vec2 sceneR = resolution;
    sceneR.x *= resolution.y / resolution.x;
    vec2 fragCoordScene = TexCoords * sceneR;

    // -------- 파동 / whiteColor용 종횡비 반대 보정 --------
    vec2 waveR = resolution;

    vec2 fragCoordWave = TexCoords * waveR;

    // 파동 벡터 기준 계산 (white 기준이므로 waveR 기준)
    vec2 U = ((2.0 * fragCoordWave) - waveR) / min(waveR.x, waveR.y);
    float D = length(U);
    vec2 dir = D > 0.0 ? (U / D) : vec2(0.0);
    vec2 T = fragCoordScene / sceneR.y;

    // 시간 기반 파동 계산
    float frame_time = mod(uTime * uSpeed, uPeriod);
    float pixel_time = max(0.0, frame_time - D);
    float wave_height = (cos(pixel_time * uFreq) + 1.0) / 2.0;
    float wave_scale = 1.0 - min(1.0, pixel_time / uFade);
    float frac = wave_height * wave_scale;

    if (mod(uTime * uSpeed, uPeriod * 2.0) > uPeriod) {
        frac = 1.0 - frac;
    }

    // 텍스처 좌표 왜곡
    vec2 tc = T + (dir * -(sin(pixel_time * uFreq) / uFade) * wave_scale * uDisplacement);

    // 샘플링 및 결과 합성
    vec4 sceneColor = texture(uSceneTexture, tc);
    vec4 whiteColor = vec4(1.0);

    FragColor = mix(whiteColor, sceneColor, frac);
}
