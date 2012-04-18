#version 110
#extension GL_ARB_texture_rectangle : enable

varying float debug;
uniform sampler2DRect tex0;
varying vec4 texCdNorm;
void main()
{
	vec4 col = texture2DRect(tex0, gl_TexCoord[0].st);
	//col = texCdNorm;
	
	col.a = 1.;
	//col.rg = gl_TexCoord[0].xy;
	gl_FragColor = col * debug;

}