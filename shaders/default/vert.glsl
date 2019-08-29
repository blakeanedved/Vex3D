#version 330 core
layout(location=0)in vec4 position;
layout(location=1)in vec2 texCoords;
layout(location=2)in vec4 normal;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * position;
}
