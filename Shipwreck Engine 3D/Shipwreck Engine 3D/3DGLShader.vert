#version 330 core

layout(location = 0) in vec4 colorIn;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec3 vertexPos;

uniform mat4 transform;
uniform mat4 camera;


struct VS_Out
{
	vec4 pos;
	vec3 worldPos;
	vec3 normal;
	vec4 color;
};


out VS_Out vsOut;

void main(){
	gl_Position = transform * camera * vec4(vertexPos, 1.0);

	vsOut.color = colorIn;
	vsOut.normal = normalize(mat3(transform) * normalIn);
	vsOut.worldPos = vertexPos;
	vsOut.pos = camera * transform * vec4(vertexPos, 1.0);
}