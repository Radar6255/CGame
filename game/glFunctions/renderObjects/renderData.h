#ifndef RENDER_DATA_HEADER
#define RENDER_DATA_HEADER

// Defining the number of buffers that we are going to have for each object
#define RENDER_DATA_BUFFERS 4

struct renderData{
    float points[6][3];
    float texCoords[6][2];
};

struct renderObject{
    float* verticies;
    float* normals;
    float* texCoords;
    unsigned int* indicies;

    long numVerts;
    long numTris;
};

#endif
