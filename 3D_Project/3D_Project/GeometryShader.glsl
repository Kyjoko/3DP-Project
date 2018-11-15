#version 440

layout(location = 3) uniform mat4 mat_world;
layout(location = 4) uniform mat4 mat_view;
layout(location = 5) uniform mat4 mat_projection;

in vec3 vertexNormalGs[];
in vec3 vertexPositionGs[];

out vec3 vertexNormal;
out vec3 vertexPosition;
out vec3 view_pos;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main(){
	mat4 wvp = mat_projection * mat_view * mat_world;
	
	view_pos = vec3(mat_world * vec4(mat_view[3][0], mat_view[3][1], mat_view[3][2], 0.0));

	vec3 vertPosWorld = vec3(mat_world * mat_view * vec4(vertexPositionGs[0].xyz, 0.0));
	vec3 vertNormWorld = normalize(vec3(mat_world * mat_view * vec4(vertexNormalGs[0].xyz, 0.0)));
	vec3 view_dir = normalize(vec3(mat_view[0][2], mat_view[1][2], mat_view[2][2]));
	
	if(dot(view_dir, normalize(vertNormWorld)) < 0){
		for(int i = 0; i < 3; i++){
			gl_Position = wvp * gl_in[i].gl_Position;
			vertexNormal = vec3(mat_world * vec4(vertexNormalGs[i].xyz, 0.0));
			vertexPosition = vec3(mat_world * vec4(vertexPositionGs[i].xyz, 1.0));
			EmitVertex();
		}
		EndPrimitive();
	}
}