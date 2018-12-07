#version 440

out vec4 outColor;

in vec2 uv_gs;

uniform sampler2D tex;

void main(){
	outColor = vec4(1.0, 1.0, 1.0, 1.0); //texture(tex, uv_gs) + vec4(255.0, 0.0, 0.0, 1.0);
}