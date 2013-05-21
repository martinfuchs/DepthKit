
uniform sampler2DRect image;
uniform sampler2DRect waterbump;

uniform float waterDistortion;
uniform vec2 waterbumpScale;
uniform vec2 waterbumpOffset;
uniform vec2 waterbumpDimensions;


void main (void)
{
	
	vec2 texCoord = gl_TexCoord[0].st;
	vec2 waterTexCoord = mod(gl_TexCoord[0].st * waterbumpScale + waterbumpOffset, waterbumpDimensions);
	// Calculate vector from pixel to light source in screen space.
	
	vec2 waterDistort = (texture2DRect(waterbump, waterTexCoord).rg * 2.0 - 1.0) * waterDistortion * 10.0;
	texCoord += waterDistort;
	gl_FragColor = texture2DRect(image, texCoord);
}