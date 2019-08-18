#version 330 core
layout(location=0)in vec3 position;
layout(location=1)in vec2 texCoord;

out vec2 v_TexCoord;

void main(){
	gl_Position.xyz = position;
	gl_Position.w = 1.0;
	v_TexCoord = texCoord;
}
