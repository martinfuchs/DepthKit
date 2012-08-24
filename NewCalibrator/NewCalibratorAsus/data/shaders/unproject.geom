#version 120
#extension GL_EXT_geometry_shader4 : enable

varying in float VZPositionValid0[3];

void main() {
    if(VZPositionValid0[0] == 1. && VZPositionValid0[1] == 1. && VZPositionValid0[2] == 1.){
        for(int i = 0; i < gl_VerticesIn; ++i) {
            gl_Position = gl_PositionIn[i];
            gl_FrontColor = gl_FrontColorIn[i];            
            gl_TexCoord[0] = gl_TexCoordIn[i][0];
            EmitVertex();
        }
    }
}
