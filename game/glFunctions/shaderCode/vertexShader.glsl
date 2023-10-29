#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texturePosition;

uniform mat4 worldTrans;
uniform mat4 screenTrans;

out vec2 uvGeo;

void main() {
    gl_Position = screenTrans * worldTrans * vec4(vertexPosition, 1.0);
    //gl_Position = vec4(vertexPosition, 1.0) * screenTrans * worldTrans;
    //gl_Position = vec4(vertexPosition, 1.0) * worldTrans;
    //gl_Position = cameraPos * screenTrans * vec4(vertexPosition, 1.0);

    uvGeo = texturePosition;
}
