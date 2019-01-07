#version 440

out vec4 outColor;

in vec2 uvCoord;

uniform sampler2D tex;
uniform sampler2D particle;
uniform sampler2D depth;

void main(){
	
	vec4 texColor = texture(particle, uvCoord);
    if(texColor.a < 0.1)
        discard;

	outColor = texColor;
}