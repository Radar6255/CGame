#version 330
precision mediump float;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2[] uvGeo;
out vec2 uvFrag;

void main(){
    for(int i = 0; i < 3; i++){
        uvFrag = uvGeo[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
