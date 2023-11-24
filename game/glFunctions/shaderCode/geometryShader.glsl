#version 330
precision mediump float;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2[] uvGeo;
in vec3[] norm;
in vec3[] lightDir;

out vec2 uvFrag;
out vec3 outNorm;
out vec3 outLightDir;

void main(){
    for(int i = 0; i < 3; i++){
        uvFrag = uvGeo[i];
        outNorm = norm[i];
        outLightDir = lightDir[i];
        // outNorm = vec3(1.0, 1.0, 0.0);
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
