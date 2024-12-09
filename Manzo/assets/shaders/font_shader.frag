#version 450 core

uniform sampler2D u_Texture; 

in vec3 v_textureColor;  
in vec2 v_textureCoord;    

out vec4 f_Color;       

void main()
{
    vec4 texColor = texture(u_Texture, v_textureCoord); 
    f_Color = vec4(v_textureColor, texColor.r); 
}
