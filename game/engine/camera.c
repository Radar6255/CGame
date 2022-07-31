// File used to perform camera operations
#include "../glFunctions/headers/init.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <cglm/cglm.h>

// Starting camera attributes
vec3 startCameraPos = {0, 0, 0};
vec3 startCameraDirection = {1, 0, 0};
vec3 cameraUp = {0, 1, 0};

/**
 * Function to set the camera position in the 
 */
void setCameraPos(vec3 cameraPos, vec3 cameraDir, int cameraUniformPos){
    mat4 viewMat;
    glm_look(cameraPos, cameraDir, cameraUp, viewMat);
    glUniformMatrix4fv(cameraUniformPos, 1, GL_FALSE, viewMat[0]);
}

void initCamera(int cameraUniformPos){
    setCameraPos(startCameraPos, startCameraDirection, MP_VIEW_MAT);
}

// Need to make a function to adjust the camera
