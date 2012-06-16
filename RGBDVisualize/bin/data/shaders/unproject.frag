#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex0;
varying float VZPositionValid0;
void main()
{
	if(VZPositionValid0 < 0.999){ 
    	discard;
    }
     
	vec4 col = texture2DRect(tex0, gl_TexCoord[0].st);
	gl_FragColor = col * gl_Color;
    //gl_FragColor = vec4(VZPositionValid0, 0.0, 0.0, 1.0);

}