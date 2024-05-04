#version 330 core

layout(location = 0) in vec4 colorIn;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec3 vertexPos;

uniform mat4 transformIn;
uniform mat4 cameraIn;
uniform vec3 viewPosIn;

out vec4 pos;
out vec3 worldPos;
out vec3 normal;
out vec4 color;
out vec3 viewPos;

void main(){
	gl_Position = cameraIn * transformIn * vec4(vertexPos, 1.0);

	color = colorIn;
	normal = normalize(mat3(transformIn) * normalIn);
	worldPos = vertexPos;
	pos = vec4(vertexPos, 1.0) * transformIn * cameraIn;
	viewPos = viewPosIn;
}