#version 330 core
layout(location=0)out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main(){
//    color = vec4(0.4, 0.4, 0.4, 1.0);
    color = texture(u_Texture, v_TexCoord);
}
