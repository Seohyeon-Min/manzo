#version 450 core

out vec4 fragColor;
layout(location = 1)in vec2 vTextureCoordinates;

uniform float iTime;
uniform vec2 iResolution;
uniform sampler2D uTex2d;

#define PI 3.1415926

vec3 getPalette(float t){
   vec3 a = vec3(0.5, 0.5, 0.5);
   vec3 b = vec3(0.5, 0.5, 0.5);
   vec3 c = vec3(1.0, 1.0, 1.0);
   vec3 d = vec3(0.0, 0.33, 0.67);
   return a + b * cos(6.2830 * (c * t + d));  
}

mat2 rot2D(float a){
   float c = cos(a);
   float s = sin(a);
   return mat2(c, s, -s, c);
}

float hash1(vec2 p){
   vec2 v = vec2(PI * 1453.0, exp(1.0) * 3054.0);
   return fract(sin(dot(p, v) * 0.1) * 4323.0);
}

vec2 hash2(vec2 p){
   return vec2(hash1(p), hash1(p * p)); 
}

float fbm1(vec2 uv){
   float amp = 1.4;
   float freq = 3.0;
   float ret = 0.0;
   for (float i = 0.0; i < 4.0; i++) {
      ret += texture(uTex2d, (uv * freq) - iTime * 0.10).x * amp;
      freq *= 0.5;
      amp *= 2.0;
   }
   return ret;
}

void fire(inout vec3 col, vec2 uv, vec2 c, float fs){
   uv -= c;
   vec2 s = vec2(floor(mod(iTime * 1000.0, 1.5))) * 0.000; 
   uv += vec2(-s.x, s.x);

   float a = atan(uv.y, uv.x);
   float r = length(uv);
   float v = max(abs(uv.x), abs(uv.y));

   float nv = max(0.0, dot(vec2(0.8), normalize(vec2(cos(a + r * 1.0), sin(a - r * 1.0))))); 
   vec3 pcol = getPalette(0.33); 
   float gray = dot(pcol, vec3(0.299, 0.587, 0.114));
   pcol = mix(vec3(gray), pcol, 0.2); 

   v = pow(v * fs, 1.4);
   col += pcol * pow(nv, 7.0) / v;
}

void main() {
    vec2 fragCoord = vTextureCoordinates * iResolution;
    vec2 uv = fragCoord / iResolution - vec2(0.5);
    uv.x *= iResolution.x / iResolution.y;

    vec3 col = vec3(0);
    float fs = fbm1(uv);
    fire(col, uv, vec2(0.0, 0.0), fs);
    col = smoothstep(0.0, 1.0, col);

    fragColor = vec4(col, 1.0);
}
