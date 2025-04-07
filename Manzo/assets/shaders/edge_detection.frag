#version 450 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTextureCoordinates;

layout(location = 0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec2 textureSize;  
uniform bool canCollide;
uniform bool isColliding;

void main() {
    vec2 texelSize = vec2(1.0 / textureSize.x, 1.0 / textureSize.y);
    vec4 tex_color = texture(uTex2d, vTextureCoordinates);
    if (tex_color.a <= 0)
        discard;

    if (isColliding && canCollide) {
        float alpha = tex_color.a;

        // get alpha value of nearest pixel 
        float alphaLeft   = texture(uTex2d, vTextureCoordinates - vec2(texelSize.x, 0)).a;
        float alphaRight  = texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0)).a;
        float alphaTop    = texture(uTex2d, vTextureCoordinates + vec2(0, texelSize.y)).a;
        float alphaBottom = texture(uTex2d, vTextureCoordinates - vec2(0, texelSize.y)).a;

        // edge detection
        float edge = abs(alphaLeft - alphaRight) + abs(alphaTop - alphaBottom);
        edge = smoothstep(0.02, 0.9, edge);  // intensity

        // Gaussian blur
        float blurStrength = 0.7;
        vec4 blurredColor = vec4(0.0);

        float weight[9];
        weight[0] = 1.0 / 16.0; weight[1] = 2.0 / 16.0; weight[2] = 1.0 / 16.0;
        weight[3] = 2.0 / 16.0; weight[4] = 4.0 / 16.0; weight[5] = 2.0 / 16.0;
        weight[6] = 1.0 / 16.0; weight[7] = 2.0 / 16.0; weight[8] = 1.0 / 16.0;

        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, texelSize.y)) * weight[0];  // top left
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(0.0, texelSize.y)) * weight[1];  // top
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, texelSize.y)) * weight[2];  // top right

        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, 0.0)) * weight[3];  // left
        blurredColor += tex_color * weight[4];  // center
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, 0.0)) * weight[5];  // right

        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(-texelSize.x, -texelSize.y)) * weight[6];  // bottom left
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(0.0, -texelSize.y)) * weight[7];  // bottom
        blurredColor += texture(uTex2d, vTextureCoordinates + vec2(texelSize.x, -texelSize.y)) * weight[8];  // bottom right

        // gaussian blur intensity
        tex_color.rgb = mix(tex_color.rgb, blurredColor.rgb, blurStrength);

        // Glow
        vec3 glowColor = vec3(1.0, 0.8, 0.4);  // glow color
        float glowIntensity = edge * 1.5;  // strong application
        tex_color.rgb = mix(tex_color.rgb, glowColor, glowIntensity);
    }

    fFragClr = tex_color;
}
