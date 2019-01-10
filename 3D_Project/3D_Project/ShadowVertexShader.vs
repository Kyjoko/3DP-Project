#version 440

layout (location = 0) in vec3 position;

uniform mat4 mat_lightSpace;
uniform mat4 mat_world;

void main()
{
    gl_Position = mat_lightSpace * mat_world * vec4(position, 1.0);
}  