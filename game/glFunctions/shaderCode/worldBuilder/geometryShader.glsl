#version 330
precision mediump float;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec2[] uvGeo;
out vec2 uvFrag;

void main(){
    vec2 avgPos = vec2(0.0, 0.0);

    for(int i = 0; i < 3; i++){
        avgPos.x += gl_in[i].gl_Position.x;
        avgPos.y += gl_in[i].gl_Position.y;
        // gl_Position = gl_in[i].gl_Position;
    }

    for(int i = 0; i < 3; i++){
        vec2 diff = gl_in[i].gl_Position.xy - avgPos;
        diff = diff * 0.8;
        diff = diff + avgPos;
        uvFrag = uvGeo[i];

        gl_Position = vec4(diff, gl_in[i].gl_Position.zw);

        EmitVertex();
    }
    EndPrimitive();
}
