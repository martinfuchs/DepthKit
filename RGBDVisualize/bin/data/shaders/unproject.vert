#version 110
#extension GL_ARB_texture_rectangle : enable

uniform vec2 dim;
uniform vec2 shift;
uniform vec2 scale;

uniform sampler2DRect depthTex;
uniform vec2 principalPoint;
uniform vec2 fov;
uniform float farClip;
uniform float edgeClip;
uniform float xsimplify;
uniform float ysimplify;
uniform int useTexture;
uniform mat4 tTex;

varying float VZPositionValid0;

void main(void)
{
    //align to texture
    vec2 texCoordPixAligned = floor(gl_Vertex.xy) + vec2(.5,.5);
    
    float depth = texture2DRect(depthTex, texCoordPixAligned).r * 65535.;
    float right = texture2DRect(depthTex, texCoordPixAligned + vec2(floor(xsimplify),0.0)).r * 65535.;
    float down  = texture2DRect(depthTex, texCoordPixAligned + vec2(0.0,floor(ysimplify))).r * 65535.;
    float left  = texture2DRect(depthTex, texCoordPixAligned + vec2(floor(-xsimplify),0.0)).r * 65535.;
    float up    = texture2DRect(depthTex, texCoordPixAligned + vec2(0.0,floor(-ysimplify))).r * 65535.;
    float bl    = texture2DRect(depthTex, texCoordPixAligned + vec2(floor(-xsimplify),floor(ysimplify))).r * 65535.;
    float ur    = texture2DRect(depthTex, texCoordPixAligned + vec2(floor(xsimplify),floor(-ysimplify))).r * 65535.;

    //cull invalid verts
    VZPositionValid0 = (depth < farClip &&
                        depth > 20. && //TODO: add variable near clip
                        abs(down - depth) < edgeClip &&
                        abs(right - depth) < edgeClip &&
                        abs(bl - depth) < edgeClip &&
                        abs(up - depth) < edgeClip &&                       
                        abs(left - depth) < edgeClip &&
                        abs(ur - depth) < edgeClip
                         ) ? 1.0 : 0.0;

    //find the 3d position
	vec4 pos = vec4((gl_Vertex.x - principalPoint.x) * depth / fov.x,
                    (gl_Vertex.y - principalPoint.y) * depth / fov.y, depth, 1.0);


    //projective texture on the geometry
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
    gl_FrontColor = gl_Color;

    if(useTexture == 1){
        
        vec4 texCd = tTex * pos;
        texCd.xyz /= texCd.w;
        
        texCd.y *= -1.;
        texCd.xy += 1.;
        texCd.xy /= 2.;
        
        texCd.xy *= scale;
        texCd.xy += shift;
        
        texCd.xy *= dim;
        gl_TexCoord[0] = texCd;
    }
}
