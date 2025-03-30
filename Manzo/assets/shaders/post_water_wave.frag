#version 450 core

layout(location = 0) in vec2 TexCoords; // Vertex에서 전달된 텍스처 좌표

out vec4 FragColor;

// Shadertoy 스타일 uniform
uniform float iTime;
uniform vec2  iResolution;
uniform vec2  iMouse;
uniform sampler2D uSceneTexture;

void main()
{
    float waveStrength    = 0.02;
    float frequency       = 30.0;
    float waveSpeed       = 5.0;
    vec4  sunlightColor   = vec4(1.0, 0.91, 0.75, 1.0);
    float sunlightStrength= 5.0;
    float centerLight     = 2.0;
    float oblique         = 0.25;

    // 마우스 위치를 0~1 범위로 계산
    vec2 tapPoint = vec2(iMouse.x / iResolution.x, iMouse.y / iResolution.y);

    // 텍스처 좌표(0~1 범위), 여기서는 Shadertoy의 fragCoord.xy / iResolution.xy에 대응
    vec2 uv = TexCoords;
    
    float modifiedTime = iTime * waveSpeed;
    float aspectRatio  = iResolution.x / iResolution.y;
    
    // 중심이 tapPoint가 되도록 거리 계산
    vec2 distVec = uv - tapPoint;
    distVec.x *= aspectRatio;
    float distance = length(distVec);

    // 범위(1.0) 안일 때만 multiplier를 계산 (바깥은 0)
    float multiplier = (distance < 1.0) ? (distance - 1.0) * (distance - 1.0) : 0.0;
    // sin 파동 계산
    float addend = (sin(frequency * distance - modifiedTime) + centerLight) 
                   * waveStrength * multiplier;
    // 약간의 왜곡(oblique)
    vec2 newTexCoord = uv + addend * oblique;

    // 밝은 빛(태양광)을 추가
    vec4 colorToAdd = sunlightColor * sunlightStrength * addend;

    // 최종 색상 = 텍스처 샘플 + 추가 조명
    FragColor = texture(uSceneTexture, newTexCoord) + colorToAdd;
}