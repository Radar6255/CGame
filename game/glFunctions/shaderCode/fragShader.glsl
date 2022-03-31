#version 460
precision mediump float;

in vec2 uvFrag;

// uniform sampler2D tex0;

// Color that is the result of this shader
out vec4 fragColor;

void main() {
    fragColor = vec4 (0.5, 0.5, 0.5, 1.0);
    // fragColor = vec4(uv.x, uv.y, 0.0, 1.0);

    // fragColor = texture(tex0, uvFrag);
}