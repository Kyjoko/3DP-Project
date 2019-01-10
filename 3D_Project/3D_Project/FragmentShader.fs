#version 440

out vec4 outColor;

in vec3 vertexNormal;
in vec3 vertexPosition;
in vec4 lightSpacePosition;
in vec2 uv_gs;

in vec3 view_pos;

const int MAX_LIGHTS = 5;
struct PointLight{
	vec3 pos;
	vec4 color;
	float radius;
};

uniform PointLight pointLights[MAX_LIGHTS];
uniform int lightCount;

layout(binding=0)uniform sampler2D depth;
layout(binding=1)uniform sampler2D tex;
layout(binding=2)uniform sampler2D particle;

float shadow;


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

		diffuse += d * pointLights[i].color * (pointLights[i].radius/distance);

		//Specular
		float specStrength = 0.5;
		int shininess = 32;
		vec3 viewDir = normalize(view_pos - vertexPosition);
		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		specular += specStrength * spec * pointLights[i].color * pointLights[i].radius;

		// Perspective divide
		vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;

		projCoords = projCoords * 0.5 + 0.5;

		// Get closest depth value from light's perspective
		float closestDepth = texture(depth, projCoords.xy).x; 

		// Get depth of current fragment from light's perspective
		float currentDepth = projCoords.z;

		// Check whether current frag pos is in shadow
		//float bias = max(0.05 * (1.0 - dot(normal, lightDir), 0.005);
		if( closestDepth < currentDepth)
		{
			shadow = 1.0;
		}
		else
		{
			shadow = 0.0;
		}

	}

	outColor = texture(depth, uv_gs) * (ambient + (1.0-shadow) * ( diffuse + specular)) * materialColor;
}