void* intializeRenderer(int size);
int addRenderObject(void* renderObjects, int x, int y, GLuint vaoIndex);
char removeRenderObject(void* renderObjects, unsigned int renderObjectId);
void renderAllObjects(void* renderObjects, GLuint program, GLuint screenTransUni);
void translateObject(void* renderObjects, unsigned int renderObjectId, float trans[2]);
void rotateObject(void* renderObjects, unsigned int renderObjectId, float theta);
void scaleObject(void* renderObjects, unsigned int renderObjectId, float scale[2]);
void freeRenderer(void* renderObjects);