uniform float focalDistance;
uniform float focalRange;
uniform float fogNear;
uniform float fogRange;
varying float focus;
varying float fog;
void main(void)
{
    gl_Position = ftransform();
    focus = min(abs(gl_Position.z - focalDistance) / focalRange, 1.0);
    fog = clamp( (fogNear - gl_Position.z) / fogRange, 0.0, 1.0);
}
