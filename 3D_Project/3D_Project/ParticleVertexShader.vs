#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

layout(location = 3) uniform mat4 mat_world;
layout(location = 4) uniform mat4 mat_view;
layout(location = 5) uniform mat4 mat_projection;

out vec2 uv_vs;
out vec3 view_pos;

void main(){

	mat4 wvp = mat_projection * mat_view * mat_world;
	mat4 invView = inverse(mat_view);	//CameraWorld Matrix
	vec3 camPos = vec3(invView[3][0], invView[3][1], invView[3][2]); 
	view_pos = camPos;

	gl_Position = wvp * vec4(position, 1.0f);
	
	//vertexNormal = vec3(mat_world * vec4(normal, 0.0));
	//vertexPosition = vec3(mat_world * vec4(position, 1.0));
	uv_vs = uv;
}
