#include <cglm/cglm.h>

#include "renderData.h"
// TODO Add normals and stuff like that potentially

float points[][3] = {
    {-0.5f, -0.5f, 0.0f},
    {0.5f, -0.5f, 0.0f},
    {-0.5, 0.5f, 0.0f},
    {0.5f, -0.5f, 0.0f},
    {0.5f, 0.5f, 0.0f},
    {-0.5f, 0.5f, 0.0f}
};

float texCoords[][2] = {
    {0, 0},
    {1, 0},
    {0, 1},
    {1, 0},
    {1, 1},
    {0, 1}
};

// renderData initRect(){
//     renderData rectData;
    // rectData.points = &points;

    // return rectData;
// }