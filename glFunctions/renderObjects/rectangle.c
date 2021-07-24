#include <cglm/cglm.h>

#include "renderData.h"
// TODO Add normals and stuff like that potentially

static float points[][3] = {
    {-0.5f, -0.5f, 0.0f},
    {0.5f, -0.5f, 0.0f},
    {-0.5, 0.5f, 0.0f},
    {0.5f, -0.5f, 0.0f},
    {0.5f, 0.5f, 0.0f},
    {-0.5f, 0.5f, 0.0f}
};

renderData initRect(){
    renderData rectData;
    rectData.points = points;

    return rectData;
}