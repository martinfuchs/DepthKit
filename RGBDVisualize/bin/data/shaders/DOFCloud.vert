uniform sampler2DRect src_tex_unit0;

uniform float focusDistance;
uniform float aperture;
uniform float minPointSize;
uniform float blowoutReduce;

const float PI = 3.14159265;
const float maxSize = 32.;

uniform vec2 dim;
uniform vec2 fudge;
//varying float debug;
varying vec4 texCdNorm;

void main() {
    
    mat4 tTex = gl_TextureMatrix[0];
    vec4 object = gl_Vertex;
    
    vec4 withinRGB = tTex * object;
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
    
    //gl_TexCoord[0] = texCd;

	// get the homogeneous 2d position
  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_Position = ftransform();
  
	// use the distance from the camera and aperture to determine the radius
	// the +1 is because point sizes <1 are rendered differently than those >1
  gl_PointSize = min(maxSize, abs(gl_Position.z - focusDistance) * aperture + 1.);
  
  gl_FrontColor = texture2DRect(src_tex_unit0, texCd.xy);
  
  float radius = gl_PointSize / 2.0;  
  gl_FrontColor.rgb /= vec3(blowoutReduce*radius,blowoutReduce*radius,blowoutReduce*radius);
  
	// divide the color alpha by the area
  gl_FrontColor.a /= aperture;
  gl_FrontColor.a /= PI * radius * radius;
  
  gl_PointSize += minPointSize;

}
