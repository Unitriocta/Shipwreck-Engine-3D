#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec4 colorIn;

uniform mat4 matrix;


out vec4 vertColor;

void main(){
	gl_Position = matrix * vec4(vertexPos, 1.0);
	vertColor = colorIn;
}