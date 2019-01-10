#version 440

void main()
{   
	//Passthrough happens by default
    gl_FragDepth = gl_FragCoord.z;
}  