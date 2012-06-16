
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
	VZPositionValid0 = (gl_Vertex.z < farClip && gl_Vertex.z > 200.) ? 1.0 : 0.0;

    vec4 pos = vec4( ((gl_Vertex.x - principalPoint.x) / imageSize.x) * gl_Vertex.z * fov.x,
                     ((gl_Vertex.y - principalPoint.y) / imageSize.y) * gl_Vertex.z * fov.y,
                      gl_Vertex.z, 1.0);
    
    
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
    
    VInFocus0 = min(abs(gl_Position.z - focalDistance) / focalRange, 1.0);
}
