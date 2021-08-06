#include <cglm/cglm.h>
#include <string.h>

#include "renderData.h"
// TODO Add normals and stuff like that potentially

static float points[6][3] = {
    {-0.5f, -0.5f, 0.0f},
    {0.5f, -0.5f, 0.0f},
    {-0.5, 0.5f, 0.0f},
    {0.5f, -0.5f, 0.0f},
    {0.5f, 0.5f, 0.0f},
    {-0.5f, 0.5f, 0.0f}
};

static float texCoords[6][2] = {
    {0, 0},
    {1, 0},
    {0, 1},
    {1, 0},
    {1, 1},
    {0, 1}
};

struct renderData *initRect(){
    struct renderData* rectData = malloc(sizeof(struct renderData));
    memcpy(rectData->points, points, sizeof(points));
    memcpy(rectData->texCoords, texCoords, sizeof(texCoords));

    return rectData;
}