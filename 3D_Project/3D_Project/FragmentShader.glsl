#version 440

layout(location = 0) out vec4 color;

in vec4 vertexNormal;

void main(){
	
	color = vec4(1.0, 0.0, 0.0, 1.0);
	//color = vertexNormal;

	vec4 lightDir = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	float intensity = 0.2;

	float d = dot(vertexNormal, lightDir);
	d = max(d, 0);
	color = (color + lightColor) * d * intensity;
}