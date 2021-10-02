#include <GL/glew.h>
#include <GL/gl.h>

#include "../renderObjects/renderData.h"

void initGL();
GLuint initProgram(char* vertShaderLoc, char* fragShaderLoc);
GLuint* getUniformLocations(GLuint program, int numUniforms, const char** uniformNames);
void bindVAO(struct renderData* data, GLuint vao, GLuint program);
void freeGLResources();
// Sets up the projection matrix for the graphics calculations
// Needs to be called on every window update due to it needing the aspect ratio
void setProjMat(int windowWidth, int windowHeight);
GLuint getVAO(int index);
// Returns the main program if it has been initialized
GLuint getMainProgram();
GLuint uniformPosition(int index);