#include <GL/glew.h>
#include <GL/gl.h>
#include <cglm/cglm.h>

#include "../renderObjects/renderData.h"

void initGL();

GLuint initProgram(char* vertShaderLoc, char* geometryShaderLoc, char* fragShaderLoc);

GLuint* getUniformLocations(GLuint program, int numUniforms, const char** uniformNames);

void bindVAO(struct renderObject* data, GLuint vao, GLuint program);

void freeGLResources();

// Sets up the projection matrix for the graphics calculations
// Needs to be called on every window update due to it needing the aspect ratio
void setProjMat(int windowWidth, int windowHeight);

// Sets the camera position
void setCameraPos(vec3 cameraPos, vec3 cameraDir);

GLuint getVAO(int index);

// Returns the main program if it has been initialized
GLuint getMainProgram();

GLuint uniformPosition(int index);