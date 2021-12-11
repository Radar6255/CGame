#ifndef WB_INIT
#define WB_INIT

#include <GL/glew.h>
#include <GL/gl.h>

// Holds the location of the uniform locations in MAIN_PROGRAM_UNIFORMS.
// Naming convention MP(Main Program), underscores for space and all caps
enum worldBuilderProgramUniforms{
    WBP_TEX0,
    WBP_WORLD_T,
    WBP_NUM_UNIFORMS
};


GLuint getWBUniformLoc(int index);
GLuint getWorldBuilderProgram();
void* getWBRenderer();
GLuint getWBVAOArray(int index);
void loadWBProgram();

#endif