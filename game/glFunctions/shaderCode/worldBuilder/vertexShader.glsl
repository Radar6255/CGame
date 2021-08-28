#version 460

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texturePosition;

out vec2 uv;

void main() {
    gl_Position = vec4(vertexPosition, 1.0);

    uv = texturePosition;
}