#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texturePosition;

uniform mat4 worldTrans;
uniform mat4 screenTrans;

out vec2 uvGeo;
out vec3 norm;
out vec3 lightDir;

void main() {
    gl_Position = screenTrans * worldTrans * vec4(vertexPosition, 1.0);
    //gl_Position = vec4(vertexPosition, 1.0) * screenTrans * worldTrans;
    //gl_Position = vec4(vertexPosition, 1.0) * worldTrans;
    //gl_Position = cameraPos * screenTrans * vec4(vertexPosition, 1.0);

    // norm = vec3(1.0, 1.0, 0.0);
    norm = (worldTrans * vec4(normal, 1.0)).xyz;
    uvGeo = texturePosition;

    lightDir = (worldTrans * vec4(0.3, 0.7, 0.3, 1.0)).xyz;
}
