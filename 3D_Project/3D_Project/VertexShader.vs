#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 mat_world;
uniform mat4 mat_view;
uniform mat4 mat_projection;
uniform mat4 mat_lightSpace;

out vec3 vertexNormalGs;
out vec3 vertexPositionGs;
out vec4 lightSpacePosition;
out vec2 uv_vs;

void main(){

	gl_Position = mat_world * vec4(position, 1.0f);
	lightSpacePosition = mat_lightSpace * gl_Position;
	vertexNormalGs = vec3(mat_world * vec4(normal, 0.0f));
	vertexPositionGs = vec3(mat_world * vec4(position, 1.0f));
	uv_vs = uv;
}
