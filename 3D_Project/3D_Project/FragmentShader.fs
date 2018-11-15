#version 440

out vec4 outColor;

in vec3 vertexNormal;
in vec3 vertexPosition;

in vec3 view_pos;


const int MAX_LIGHTS = 5;
struct PointLight{
	vec3 pos;
	vec4 color;
	float radius;
};

uniform PointLight pointLights[MAX_LIGHTS];
uniform int lightCount;

void main(){

	vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 materialColor = vec4(1.0, 1.0, 1.0, 1.0);

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = diffuse;
	for(int i = 0; i < lightCount; i++){
		//Diffuse
		vec3 lightPos = pointLights[i].pos;

		vec3 lightDir = lightPos - vertexPosition;
		float distance = length(lightDir);
		lightDir = normalize(lightDir);

		vec3 normal = normalize(vertexNormal);
		float d = dot(normal, lightDir);
		d = max(d, 0.0);

		diffuse += d * pointLights[i].color * (1/distance);


		//Specular
		float specStrength = 0.5;
		int shininess = 32;
		vec3 viewDir = normalize(view_pos - vertexPosition);
		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		specular += specStrength * spec * pointLights[i].color;
	}

	outColor = (ambient + diffuse + specular) * materialColor;
}