#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect colorTex;
uniform int useTexture;
varying float VZPositionValid0;

void main()
{
    if(VZPositionValid0 < 0.9999999){
    	discard;
    }
    
    if(useTexture == 1){
        vec4 col = texture2DRect(colorTex, gl_TexCoord[0].st);
        gl_FragColor = col * gl_Color;
    }
    else{
        gl_FragColor = vec4(0);
    }
    //gl_FragColor = vec4(VZPositionValid0, col.g, col.b, 1.0);
    //gl_FragColor = vec4(depthSample, depthSample, depthSample,1.0);
}