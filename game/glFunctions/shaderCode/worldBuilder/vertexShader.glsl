#version 460

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texturePosition;

uniform mat3 screenTransform;
out vec2 uvGeo;

void main() {
    gl_Position = vec4(screenTransform * vertexPosition, 1.0);

    uvGeo = texturePosition;
}