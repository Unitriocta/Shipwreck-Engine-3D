#version 330 core

in vec4 pos;
in vec3 worldPos;
in vec3 normal;
in vec4 color;

out vec4 fragColor;

vec3 lightPos = vec3(0.0, 7.0, -5.0);
vec3 ambient = vec3(0.05, 0.05, 0.05);
vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
float diffuseIntensity = 2.0;

vec3 sampleCol = vec3(0.5, 0.5, 0.5);

float attConst = 1.0;
float attLin = 0.045;
float attQuad = 0.0075;


void main()
{
	lightPos = vec3(lightPos.x, lightPos.y, -lightPos.z);

	vec3 lightOffset = lightPos - worldPos;
	float _distance = length(lightOffset);
	vec3 direction = normalize(lightOffset);

	float att = 1.0 / (attConst + attLin * _distance + attQuad * (_distance * _distance));

	vec3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0, dot(direction, normal));

	gl_FragColor = vec4(sampleCol * clamp(diffuse + ambient, 0.0, 1.0), 1.0);
}