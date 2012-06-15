
//projection uniforms
uniform vec2 principalPoint;
uniform vec2 imageSize;
uniform vec2 fov;
uniform float farClip;

//focal uniforms
uniform float focalDistance;
uniform float focalRange;
uniform float fogNear;
uniform float fogRange;


varying float VInFocus0;
varying float VZPositionValid0;
void main(void)
{
	if(gl_Vertex.z < farClip && gl_Vertex.z > 200.){
        vec4 pos = vec4( ((gl_Vertex.x - principalPoint.x) / imageSize.x) * gl_Vertex.z * fov.x,
                        ((gl_Vertex.y - principalPoint.y) / imageSize.y) * gl_Vertex.z * fov.y,
                        gl_Vertex.z, 1.0);
        
        //projective texture on the 
        gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
        gl_FrontColor = gl_Color;
        VZPositionValid0 = 1.;
    }
    else {
        gl_FrontColor = vec4(0.0);
        gl_Position = vec4(0.0);
        VZPositionValid0 = 0.;
    }    
    VInFocus0 = min(abs(gl_Position.z - focalDistance) / focalRange, 1.0);
}
