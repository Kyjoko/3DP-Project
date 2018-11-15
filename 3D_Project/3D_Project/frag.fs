#version 440

out vec4 outColor;

const int MAX_LIGHTS = 5;
struct PointLight{
	vec3 pos;
	vec4 color;
	float radius;
};

uniform PointLight pointLights[MAX_LIGHTS];
uniform int lightCount;
void main(){
	outColor = vec4(0.0, 1.0, 1.0, 1.0);
	//outColor = pointLights[0].color;
}