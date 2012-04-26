uniform vec2 dim;
uniform vec2 fudge;

void main(void)
{
	gl_Position = ftransform();
    
	mat4 tTex = gl_TextureMatrix[0];
	vec4 texCd = tTex * gl_Vertex;
	texCd.xyz /= texCd.w;
	
	texCd.xy += 1.;
	texCd.xy /= 2.;
	texCd.y = 1. - texCd.y;
    texCd.x = 1. - texCd.x;
	texCd.xy += fudge;
    
	//debug = -tTex[3][2] > 20. ? 1. : 0.;
	texCd.xy *= dim;
	gl_TexCoord[0] = texCd;
}
