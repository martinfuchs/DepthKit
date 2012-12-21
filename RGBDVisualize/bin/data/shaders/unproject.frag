#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect colorTex;
uniform int useTexture;
varying float VZPositionValid0;
const float epsilon = 1e-6;
void main()
{
    if(useTexture == 1){
        vec4 col = texture2DRect(colorTex, gl_TexCoord[0].st);
        gl_FragColor = col * gl_Color;
    }
    else{
        gl_FragColor = vec4(0);
    }    
    
    if(VZPositionValid0 < 1.0-epsilon){
    	discard;
    }

}