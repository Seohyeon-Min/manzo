#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D uSceneTexture;
uniform float uTime;
uniform vec2 uResolution;

// Parameters
uniform float uFreq = 8.0;
uniform float uPeriod = 8.0;
uniform float uSpeed = 2.0;
uniform float uFade = 4.0;
uniform float uDisplacement = 0.2;

void main() {
    vec2 R = uResolution;
    vec2 fragCoord = TexCoords * R;

    vec2 U = ((2.0 * fragCoord) - R) / min(R.x, R.y);
    vec2 T = fragCoord / R.y;
    float D = length(U);

    float frame_time = mod(uTime * uSpeed, uPeriod);
    float pixel_time = max(0.0, frame_time - D);

    float wave_height = (cos(pixel_time * uFreq) + 1.0) / 2.0;
    float wave_scale = (1.0 - min(1.0, pixel_time / uFade));
    float frac = wave_height * wave_scale;

    if (mod(uTime * uSpeed, uPeriod * 2.0) > uPeriod) {
        frac = 1.0 - frac;
    }

    // Avoid NaN when D == 0
    vec2 dir = D > 0.0 ? (U / D) : vec2(0.0);

    vec2 tc = T + (dir * -(sin(pixel_time * uFreq) / uFade) * wave_scale * uDisplacement);

    vec4 sceneColor = texture(uSceneTexture, tc);
    vec4 whiteColor = vec4(1.0);

    FragColor = mix(sceneColor, whiteColor, frac);
}
