#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 normal;

out vec3 vertexNormalGs;
out vec3 vertexPositionGs;
out vec2 uv_vs;

void main(){

	gl_Position = position;
	
	vertexNormalGs = normal.xyz;
	vertexPositionGs = position.xyz;
	uv_vs = uv;
}
