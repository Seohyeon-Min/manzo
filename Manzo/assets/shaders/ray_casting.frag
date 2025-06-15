#version 330 core
in vec2 vTex;
out vec4 FragColor;

uniform vec2 uLightPos;
uniform float uLightRadius;
uniform vec2 uScreenSize;
uniform sampler2D uObstacleMap;

vec3 uLightColor = {1.0 ,1.0, 1.0};

bool isObstacleAt(vec2 screenPos){
    vec2 uv = screenPos / uScreenSize;
    float alpha = texture(uObstacleMap, uv).a;
    return alpha > 0.5;
}

void main(){
    vec2 fragPos = vTex * uScreenSize;
    vec2 dir = normalize(fragPos - uLightPos);
    float dist = length(fragPos - uLightPos);
    if(dist > uLightRadius) discard;

    for(float t=0.0; t<dist; t+=4.0){
        vec2 sp = uLightPos + dir*t;
        if(isObstacleAt(sp)){
            discard;
        }
    }
    float att = 1.0 - dist / uLightRadius;
    FragColor = vec4(uLightColor * att,1.0);
}
