// File used to perform camera operations
#include "../glFunctions/headers/init.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <cglm/cglm.h>

// Starting camera attributes
vec3 currentCameraPos = {0, 0, 0};
vec3 currentCameraDirection = {1, 0, 0};
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
    setCameraPos(currentCameraPos, currentCameraDirection, MP_VIEW_MAT);
}

// The direction here is specified with the first element being the 
void moveCamera(vec3 dir){
    // First calculating how the camera should be translated
    vec3 worldCoord;
    // Holds the 2D coords of the camera direction, used since the player doesn't have control of what direction is up
    vec2 planeCoords = {currentCameraDirection[0], currentCameraDirection[2]};

    // Need to first account for the forward direction
    vec2 tmp;
    glm_vec2_scale(planeCoords, dir[0], tmp);
    worldCoord[0] = tmp[0];
    worldCoord[2] = tmp[1];

    // Accounting for any upwards direction
    worldCoord[1] = dir[1];

    // Need to get the perpendicular plane coords
    vec2 perpPlaneCoords = {-currentCameraDirection[2], currentCameraDirection[0]};
    glm_vec2_scale(perpPlaneCoords, dir[2], tmp);
    worldCoord[0] += tmp[0];
    worldCoord[2] += tmp[1];

    // Translating the camera position according to the direction it should move
    glm_vec3_add(currentCameraPos, worldCoord, currentCameraPos);
}
