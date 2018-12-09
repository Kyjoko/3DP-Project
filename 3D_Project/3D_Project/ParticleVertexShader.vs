#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

layout(location = 3) uniform mat4 mat_world;
layout(location = 4) uniform mat4 mat_view;
layout(location = 5) uniform mat4 mat_projection;

out vec2 uvCoord;

void main(){

	mat4 wvp = mat_projection * mat_view;

	gl_Position =  wvp * vec4(position, 1.0f);

	uvCoord = uv;
}
