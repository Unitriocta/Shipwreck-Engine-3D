#version 330 core

struct VS_Out
{
	vec4 pos;
	vec3 worldPos;
	vec3 normal;
	vec4 color;
};

uniform vec3 lightPos = vec3(0.0, 7.0, -5.0);
uniform vec3 ambient = vec3(0.05, 0.05, 0.05);
uniform vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
uniform float diffuseIntensity = 2.0;

uniform vec3 sampleCol = vec3(0.5, 0.5, 0.5);

uniform float attConst = 1.0;
uniform float attLin = 0.045;
uniform float attQuad = 0.0075;

in VS_Out fragInput;

out vec4 fragColor;

void main()
{
	vec3 lightOffset = lightPos - fragInput.worldPos;
	float _distance = length(lightOffset);
	vec3 direction = normalize(lightOffset);

	float att = 1.0 / (attConst + attLin * _distance + attQuad * (_distance * _distance));

	vec3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0, dot(direction, fragInput.normal));

	fragColor = vec4(sampleCol * clamp(diffuse + ambient, 0.0, 1.0), 1.0);
}