#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

uniform vec2 iResolution; // 화면 해상도
uniform float iTime;      // 시간

out vec4 fragColor; // 최종 출력 색상

const mat2 myt = mat2(.12121212, .13131313, -.13131313, .12121212);
const vec2 mys = vec2(1e4, 1e6);

vec2 rhash(vec2 uv) {
    uv *= myt;
    uv *= mys;
    return fract(fract(uv / mys) * uv);
}

float voronoi2d(const in vec2 point) {
    vec2 p = floor(point);
    vec2 f = fract(point);
    float res = 0.0;
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            vec2 b = vec2(i, j);
            vec2 r = vec2(b) - f + rhash(p + b);
            res += 1.0 / pow(dot(r, r), 8.0);
        }
    }
    return pow(1.0 / res, 0.0625);
}

vec2 cart2polar(vec2 uv) {
    float phi = atan(uv.y, uv.x);
    float r = length(uv);
    return vec2(phi, r);
}

void main() {
    vec2 fragCoord = gl_FragCoord.xy; // 화면 좌표
    vec2 uv = fragCoord / iResolution.xy;

    // UV 좌표 변환
    vec2 p = uv;
    p.x *= iResolution.x / iResolution.y;

    uv = (uv - 0.7) * 2.0;
    uv.x *= iResolution.x / iResolution.y;

    uv.y -= 2.0;
    uv /= 50.0;
    uv = cart2polar(uv);

    // Voronoi 노이즈 계산
    float n1 = voronoi2d((vec2(uv.x, 0.0) + 0.04 * iTime) * 8.0);
    float n2 = voronoi2d((vec2(0.1, uv.x) + 0.04 * iTime * 1.5) * 5.0);
    float n3 = min(n1, n2);

    // Alpha 및 투명도 계산
    float mask = smoothstep(.15, .96, p.y);
    float alpha = n3 * mask * 0.5;

    // 빛 효과 색상
    vec3 lightColor = mix(vec3(1, 1, 1), vec3(1.0), alpha); //0.0, 0.412, 0.580

    // 빛 외의 영역은 투명하게 설정
    fragColor = vec4(lightColor, alpha); // Alpha를 적용
}
