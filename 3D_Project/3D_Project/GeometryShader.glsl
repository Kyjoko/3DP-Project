#version 440

layout(location = 3) uniform mat4 mat_world;
layout(location = 4) uniform mat4 mat_view;
layout(location = 5) uniform mat4 mat_projection;

in vec3 vertexNormalGs[];
in vec3 vertexPositionGs[];
in vec2 uv_vs[];

out vec3 vertexNormal;
out vec3 vertexPosition;
out vec2 uv_gs;

out vec3 view_pos;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {

	mat4 wvp = mat_projection * mat_view * mat_world;
	mat4 invView = inverse(mat_view);	//CameraWorld Matrix
	vec3 camPos = vec3(invView[3][0], invView[3][1], invView[3][2]); //Position of camera in world

	//view_pos = vec3(mat_world * vec4(mat_view[3][0], mat_view[3][1], mat_view[3][2], 0.0));
	view_pos = camPos;
	//vec3 view_dir = normalize(vec3(mat_world * mat_view * mat_projection * vec4(vec3(mat_view[0][2], mat_view[1][2], mat_view[2][2]), 0.0)));
	
	vec3 vertPosWorld = vec3(mat_world * gl_in[0].gl_Position);
	vec3 v_1 = vec3(mat_world * (vec4(gl_in[1].gl_Position) - vec4(gl_in[0].gl_Position)));
	vec3 v_2 = vec3(mat_world * (vec4(gl_in[2].gl_Position) - vec4(gl_in[0].gl_Position)));
	vec3 normal = cross(v_1, v_2); //Calculate normal of primitive

	vec3 camToObj = normalize(vertPosWorld - camPos);

if(dot(camToObj, normal) < 0) { //Check direction primitive is facing

	for(int i = 0; i < 3; i++) {

		gl_Position = wvp * gl_in[i].gl_Position;
		vertexNormal = vec3(mat_world * vec4(vertexNormalGs[i].xyz, 0.0));
		vertexPosition = vec3(mat_world * vec4(vertexPositionGs[i].xyz, 1.0));
		uv_gs = uv_vs[i];
		EmitVertex();
	}

	EndPrimitive();

	}
}