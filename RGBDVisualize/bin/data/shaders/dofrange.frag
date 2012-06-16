#version 110
#extension GL_ARB_texture_rectangle : enable

varying float VInFocus0;
//hack for self-occlude
uniform float blackout;
varying float VZPositionValid0;
void main()
{
	if(VZPositionValid0 < 0.999){ 
    	discard;
    }	
	gl_FragColor = vec4(VInFocus0) * blackout;
}