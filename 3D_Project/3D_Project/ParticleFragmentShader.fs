#version 440

out vec4 outColor;

in vec2 uvCoord;

layout(binding=0)uniform sampler2D depth;
layout(binding=1)uniform sampler2D tex;
layout(binding=2)uniform sampler2D particle;

void main(){
	
	vec4 texColor = texture(particle, uvCoord);
    if(texColor.a < 0.1)
        discard;

	outColor = texColor;
}