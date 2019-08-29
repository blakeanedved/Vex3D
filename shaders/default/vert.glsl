#version 330 core
layout(location=0)in vec4 position;
layout(location=1)in vec2 texCoords;
layout(location=2)in vec4 normal;

uniform mat4 MVP;
uniform vec3 pos;
uniform mat4 rotMatrix;

void main(){
	gl_Position = MVP * (vec4(pos, 1.0) + (rotMatrix * position));
}
