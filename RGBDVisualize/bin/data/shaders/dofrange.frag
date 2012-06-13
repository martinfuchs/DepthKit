#version 110
#extension GL_ARB_texture_rectangle : enable

varying float VOutFocus0;
//varying float fog;
void main()
{
	gl_FragColor = vec4(VOutFocus0);
}