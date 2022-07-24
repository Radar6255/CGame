#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texturePosition;

uniform mat4 cameraPos;
uniform mat4 screenTrans;

out vec2 uvGeo;

void main() {
    gl_Position = vec4(vertexPosition, 1.0) * cameraPos * screenTrans;

    uvGeo = texturePosition;
}
