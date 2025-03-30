#version 450 core

uniform float iTime;
uniform vec2 iResolution;

in vec2 TexCoords;
out vec4 FragColor;

#define PI     3.1415926
#define TWO_PI 6.2831853

// mod289 함수들
float mod289(float x) { 
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec2 mod289(vec2 x) { 
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec3 mod289(vec3 x) { 
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec4 mod289(vec4 x) { 
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

// permute 함수들
float permute(float x) { 
    return mod289(((x * 34.0) + 1.0) * x);
}
vec2 permute(vec2 x) { 
    return mod289(((x * 34.0) + 1.0) * x);
}
vec3 permute(vec3 x) { 
    return mod289(((x * 34.0) + 1.0) * x);
}
vec4 permute(vec4 x) { 
    return mod289(((x * 34.0) + 1.0) * x);
}

/*============================== SimplexNoise ==============================*/
// 2D Simplex Noise
float snoise(vec2 v) {
    vec4 C = vec4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);
    vec2 i = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);
    vec2 i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod289(i); // truncation 방지를 위한 permutation 처리
    vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0)) + i.x + vec3(0.0, i1.x, 1.0));
    
    vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
    m = m * m;
    m = m * m;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);
    vec3 g;
    g.x = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

float fbm(vec2 uv, int oct) {
    vec2 pos = uv;
    float amp = 1.0, val = 0.0;
    for (int i = 0; i < oct; i++) {
        val += amp * snoise(pos);
        pos *= 2.0;
        amp *= 0.5;
    }
    return val;
}

// 3D Simplex Noise
float snoise(vec3 v) {
    vec2 C = vec2(1.0 / 6.0, 1.0 / 3.0);
    vec4 D = vec4(0.0, 0.5, 1.0, 2.0);
    vec3 i = floor(v + dot(v, C.yyy));
    vec3 x0 = v - i + dot(i, C.xxx);
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min(g.xyz, l.zxy), i2 = max(g.xyz, l.zxy);
    vec3 x1 = x0 - i1 + C.xxx;
    vec3 x2 = x0 - i2 + C.yyy;
    vec3 x3 = x0 - D.yyy;
    i = mod289(i);
    vec4 p = permute(permute(permute(i.z + vec4(0.0, i1.z, i2.z, 1.0)) 
             + i.y + vec4(0.0, i1.y, i2.y, 1.0)) 
             + i.x + vec4(0.0, i1.x, i2.x, 1.0));
    float n_ = 0.142857142857;
    vec3 ns = n_ * D.wyz - D.xzx;
    vec4 j = p - 49.0 * floor(p * ns.z * ns.z);
    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_);
    vec4 x = x_ * ns.x + ns.yyyy;
    vec4 y = y_ * ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);
    vec4 b0 = vec4(x.xy, y.xy);
    vec4 b1 = vec4(x.zw, y.zw);
    vec4 s0 = floor(b0) * 2.0 + 1.0;
    vec4 s1 = floor(b1) * 2.0 + 1.0;
    vec4 sh = -step(h, vec4(0, 0, 0, 0));
    vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww;
    vec3 p0 = vec3(a0.xy, h.x);
    vec3 p1 = vec3(a0.zw, h.y);
    vec3 p2 = vec3(a1.xy, h.z);
    vec3 p3 = vec3(a1.zw, h.w);
    vec4 norm = 1.79284291400159 - 0.85373472095314 * 
                vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;
    vec4 m = max(0.6 - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    m = m * m;
    return 42.0 * dot(m * m, vec4(dot(p0, x0), dot(p1, x1), dot(p2, x2), dot(p3, x3)));
}

// TouchDesigner 스타일 noise 함수 (자세한 파라미터 사용)
float noiseTD(vec2 uv, int seed, vec3 transform, float period, int harmonics, float spread, float gain, float exponent, float offset, float amplitude) {
    float frequency = 1.0 / period;
    float noise = 0.0;
    for (int i = 0; i < harmonics; i++) {
        noise += snoise(vec3(uv * frequency, float(seed)) + transform) * amplitude;
        frequency *= spread;
        amplitude *= gain;
    }
    noise = sign(noise) * pow(abs(noise), exponent);
    noise += offset;
    return noise;
}
// Overloaded noiseTD: exponent 기본값 1.0
float noiseTD(vec2 uv, int seed, float time, float period) {
    return noiseTD(uv, seed, vec3(0.0, 0.0, time), period, 2, 2.0, 0.7, 1.0, 0.5, 0.5);
}
// Overloaded noiseTD: exponent 지정
float noiseTD(vec2 uv, int seed, float time, float period, float exponent) {
    return noiseTD(uv, seed, vec3(0.0, 0.0, time), period, 2, 2.0, 0.7, exponent, 0.5, 0.5);
}

// 팔레트 함수
vec3 pal(float t, vec3 a, vec3 b, vec3 c, vec3 d) {
    return a + b * cos(TWO_PI * (c * t + d));
}

void main() {
    // TexCoords(0~1)를 이용해 실제 픽셀 좌표 계산
    vec2 fragCoord = TexCoords * iResolution;
    
    // UV 좌표: fragCoord를 iResolution 중 최소값으로 나눔
    vec2 uv = fragCoord / min(iResolution.x, iResolution.y);
    
    float t = iTime * 0.02;
    float period = 1.7;
    float exponent = 0.5;
    vec2 dispUV;
    dispUV.x = noiseTD(uv, 1, t, period, exponent);
    dispUV.y = noiseTD(uv, 2, t, period, exponent);
    
    // UV 좌표를 dispUV로 왜곡한 후 noise 계산
    float n = noiseTD(uv + dispUV * 0.5, 2, t * 3.0, 1.0);
    
    // 색상 정의 (0~1 범위)
    vec3 col1 = vec3(0.00392, 0.81176, 0.85098); // rgb(1, 207, 217)
    vec3 col2 = vec3(0.00392, 0.69020, 0.99608);  // rgb(1, 176, 254)
    
    // 두 색상을 noise 값에 따라 혼합
    vec3 color = mix(col1, col2, n);
    
    FragColor = vec4(color, 1.0);
}
