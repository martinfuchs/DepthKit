uniform vec2 dim;
uniform vec2 fudge;
varying float debug;
varying vec4 texCdNorm;

void main(void)
{
	debug = 1.;
	mat4 tTex = gl_TextureMatrix[0];
	gl_Position = ftransform();
	vec4 object = gl_Vertex;
	
	vec4 withinRGB = tTex *object;
	withinRGB.rgb /= withinRGB.w;
	
	withinRGB.xy += 1.;
	withinRGB.xy /= 2.;
	withinRGB.y = 1. - withinRGB.y;
	//withinRGB.xy += vec2(0, .043);
	withinRGB.xy += fudge;
    
	//debug = -tTex[3][2] > 20. ? 1. : 0.;
	vec4 texCd = vec4(0.);	
	texCdNorm = withinRGB;
	texCd.xy = texCdNorm.xy * dim;
		
	gl_TexCoord[0] = texCd;
}
