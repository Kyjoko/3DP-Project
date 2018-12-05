#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 vertexNormalGs;
out vec3 vertexPositionGs;
out vec2 uv_vs;

void main(){

	gl_Position = vec4(position, 1.0f);
	
	vertexNormalGs = normal;
	vertexPositionGs = position;
	uv_vs = uv;
}
