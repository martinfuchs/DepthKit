#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect colorTex;
uniform int useTexture;
uniform vec4 nonTextureColor;

uniform vec2 dim;
uniform float fadeAmount;
uniform vec4 fadeColor;

varying float VZPositionValid0;
const float epsilon = 1e-6;

varying vec3 normal;
varying vec4 normalColor;
varying vec3 eyeVec;

uniform float lightEffect;
uniform float shininess;

varying float diffuseAttenuate;
varying float specularAttenuate;
varying vec3 diffuseLightDirection;
varying vec3 specularLightDirection;

float calculateLight(){
	vec3 N = normal;
	vec3 L = diffuseLightDirection;
	
	float lambertTerm = dot(N,L) * diffuseAttenuate;
	return lambertTerm;
}

float calculateSpecular(){
	if(dot(normal, specularLightDirection) < 0.0){
		return 0.0;
	}
	return specularAttenuate * pow(max(0.0, dot(reflect(-specularLightDirection, normal), eyeVec)), shininess);
}

void main()
{
    if(VZPositionValid0 < epsilon){
    	discard;
        return;
    }

    if(useTexture == 1){
        vec4 col = texture2DRect(colorTex, gl_TexCoord[0].st);
		if(lightEffect > epsilon){
			float lightAttenuate = mix(1.0, calculateLight() + calculateSpecular(), lightEffect);
			col.rgb *= lightAttenuate;
		}
        gl_FragColor = mix(col, vec4(fadeColor), fadeAmount) * gl_Color;
    }
    else{
        gl_FragColor = nonTextureColor;
    }
	
	//enable visualize clipping values
    //gl_FragColor = vec4(VZPositionValid0);
	//enable visualize texture coordinates
	//gl_FragColor = vec4(gl_TexCoord[0].s / dim.x, gl_TexCoord[0].t / dim.y, 0.0, 1.0);
	//enable visualize normals
	//gl_FragColor = normalColor;
	//gl_FragColor = vec4(vec3(lightEffect), 1.0);
}
