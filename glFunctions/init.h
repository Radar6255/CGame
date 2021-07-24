void initGL();
void freeGLResources();
// Sets up the projection matrix for the graphics calculations
// Needs to be called on every window update due to it needing the aspect ratio
void setProjMat(int windowWidth, int windowHeight);