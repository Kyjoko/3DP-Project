#version 440

out vec4 outColor;

in vec2 uv_gs;

uniform sampler2D tex;

void main(){
	outColor = texture(tex, uv_gs);
}