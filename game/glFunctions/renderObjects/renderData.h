#ifndef RENDER_DATA_HEADER
#define RENDER_DATA_HEADER

#define RENDER_DATA_BUFFERS 2

struct renderData{
    float points[6][3];
    float texCoords[6][2];
};

struct renderObject{
    float* verticies;
    float* normals;
    float* texCoords;
    unsigned int numVerts;
};

#endif