#version 440

layout(location = 2) uniform mat4 mat_world;
layout(location = 3) uniform mat4 mat_view;
layout(location = 4) uniform mat4 mat_projection;

layout(location = 0) in vec4 position;
void main(){
	mat4 wvp = mat_projection * mat_view * mat_world;

	gl_Position = wvp * position;

}