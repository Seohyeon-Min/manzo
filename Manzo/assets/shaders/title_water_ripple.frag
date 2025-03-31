#version 450 core

uniform float iTime;
uniform vec2 iResolution;
uniform sampler2D uSceneTexture;  // texture

in vec2 TexCoords; // vertex shader에서 전달된 텍스처 좌표 (필요 시 사용)

out vec4 FragColor;

vec2 hash2(vec2 p) {
    return fract(sin(vec2(dot(p, vec2(123.4, 748.6)),
                          dot(p, vec2(547.3, 659.3)))) * 5232.85324);
}

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(43.232, 75.876))) * 4526.3257);
}

float voronoi(vec2 p) {
    vec2 n = floor(p);
    vec2 f = fract(p);
    float md = 5.0;
    vec2 m = vec2(0.0);
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            vec2 g = vec2(i, j);
            vec2 o = hash2(n + g);
            o = 0.5 + 0.5 * sin((iTime + 5.038 * o) * 0.522);
            vec2 r = g + o - f;
            float d = dot(r, r);
            if (d < md) {
                md = d;
                m = n + g + o;
            }
        }
    }
    return md;
}

float ov(vec2 p) {
    float v = 0.0;
    float a = 0.4;
    for (int i = 0; i < 3; i++) {
        v += voronoi(p) * a;
        p *= 2.0;
        a *= 0.5;
    }
    return v;
}

void main() {
    // gl_FragCoord.xy를 사용하여 화면상의 좌표를 구함
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec4 colorA = texture(uSceneTexture, TexCoords);
    vec4 colorB = vec4(1.85, 1.9, 2.0, 1.0);
    FragColor = mix(colorA, colorB, smoothstep(0.0, 1.5, ov(uv * 3.0)));
}