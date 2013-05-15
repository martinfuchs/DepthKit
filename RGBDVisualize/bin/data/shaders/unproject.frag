#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect colorTex;
uniform int useTexture;
uniform vec2 dim;
uniform float fadeAmount;
uniform vec4 fadeColor;

varying float VZPositionValid0;
const float epsilon = 1e-6;

varying vec3 normal;
varying vec3 lightDir, eyeVec;
varying float att;
varying vec4 normalColor;
uniform float lightEffect;

float calculateLight(){
	
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L) * att;
	return lambertTerm;
	
	//return vec4(1.0);
    //return vec4(vec3(mix(lambertTerm, 1.0, 0.0)), 1.0);
}

void main()
{
    if(VZPositionValid0 < epsilon){
    	discard;
        return;
    }

    if(useTexture == 1){
        vec4 col = texture2DRect(colorTex, gl_TexCoord[0].st);
		//enable visualize texture coordinates
		//col = vec4(gl_TexCoord[0].s / dim.x, gl_TexCoord[0].t / dim.y, 0.0, 1.0);
        gl_FragColor = mix(col, vec4(fadeColor), fadeAmount) * mix(1.0, calculateLight(), lightEffect	) * gl_Color;
    }
    else{
        gl_FragColor = vec4(0);
    }
	
	//enable visualize clipping values
    //gl_FragColor = vec4(VZPositionValid0);
	//gl_FragColor = fadeColor;
	//gl_FragColor = normalColor;
}
