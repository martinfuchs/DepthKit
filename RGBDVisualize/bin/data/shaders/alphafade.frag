#version 110

uniform sampler2DRect self;
uniform float fadeSpeed;
void main()
{
	vec4 samp = texture2DRect(self, gl_TexCoord[0].st);
	samp.a -= fadeSpeed;
    
	gl_FragColor = samp;
}