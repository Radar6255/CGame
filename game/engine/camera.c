// File used to perform camera operations
#include "../glFunctions/headers/init.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <cglm/cam.h>
#include <cglm/io.h>
#include <stdio.h>

#include <cglm/cglm.h>

// Starting camera attributes
vec3 currentCameraPos = {-2, 0, 0};
vec3 currentCameraDirection = {1, 0, 0};
vec3 cameraUp = {0, 1, 0};

/**
 * Function to set the camera position in the 
 */
void setCameraPos(vec3 cameraPos, vec3 cameraDir, int cameraUniformPos){
    mat4 viewMat;
    /* glm_mat4_print(viewMat, stdout); */
    glm_look(cameraPos, cameraDir, cameraUp, viewMat);
    vec3 origin = {0, 0, 0};
    /* glm_lookat(cameraPos, origin, cameraUp, viewMat); */

    /* printf("Printing viewMat...\n"); */
    /* printf("%f, %f, %f, %f\n", viewMat[0][0], viewMat[0][1], viewMat[0][2], viewMat[0][3]); */
    /* printf("%f, %f, %f, %f\n", viewMat[1][0], viewMat[1][1], viewMat[1][2], viewMat[1][3]); */
    /* printf("%f, %f, %f, %f\n", viewMat[2][0], viewMat[2][1], viewMat[2][2], viewMat[2][3]); */
    /* printf("%f, %f, %f, %f\n", viewMat[3][0], viewMat[3][1], viewMat[3][2], viewMat[3][3]); */

    glUniformMatrix4fv(cameraUniformPos, 1, GL_FALSE, viewMat[0]);
}

void initCamera(int cameraUniformPos){
    setCameraPos(currentCameraPos, currentCameraDirection, cameraUniformPos);
}

void setCameraRotation(float xRot, float yRot) {
    currentCameraDirection[0] = sin(xRot);
    currentCameraDirection[1] = sin(yRot);
    currentCameraDirection[2] = cos(xRot) * cos(yRot);
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
    glm_vec3_print(currentCameraPos, stderr);
    printf("%f, %f, %f\n", currentCameraPos[0], currentCameraPos[1], currentCameraPos[2]);
}


