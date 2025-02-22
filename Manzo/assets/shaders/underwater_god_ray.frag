#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D uSceneTexture;

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
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = fragCoord / iResolution.xy;

    // UV 
    vec2 p = uv;
    p.x *= iResolution.x / iResolution.y;

    uv = (uv - 0.7) * 2.0;
    uv.x *= iResolution.x / iResolution.y;

    uv.y -= 2.0;
    uv /= 50.0;
    uv = cart2polar(uv);

    // Voronoi
    float n1 = voronoi2d((vec2(uv.x, 0.0) + 0.04 * iTime) * 1.0);
    float n2 = voronoi2d((vec2(0.1, uv.x) + 0.04 * iTime * 1.5) * 4.0);
    float n3 = min(n1, n2);

    // Alpha
    float mask = smoothstep(.05, .96, p.y);
    float brightness = n3 * mask * 0.4;         // increase light
    float alpha = n3 * mask * 1.;               // increase oqaque

    // light color
    vec3 glowColor = vec3(1.0, 0.9, 0.8) * brightness;

    vec3 color = texture(uSceneTexture, TexCoords).rgb;
    FragColor = vec4(color + glowColor, 1.0);
}