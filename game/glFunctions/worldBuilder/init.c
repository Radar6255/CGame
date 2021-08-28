
#include "../headers/init.h"
#include "../renderObjects/rectangle.h"
#include "../headers/textureLoading.h"
#include "../headers/texturesList.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdlib.h>

static GLuint worldBuilderProgram;
static GLuint* wbProgramUniforms;
static GLuint* vaoArray;

#define VERT_SHADER_LOC "game/glFunctions/shaderCode/worldBuilder/vertexShader.glsl"
#define FRAG_SHADER_LOC "game/glFunctions/shaderCode/worldBuilder/fragShader.glsl"

// Holds the location of the uniform locations in MAIN_PROGRAM_UNIFORMS.
// Naming convention MP(Main Program), underscores for space and all caps
enum worldBuilderProgramUniforms{
    WBP_TEX0,
    WBP_NUM_UNIFORMS
};

// All of the uniforms that are in the main program
// Used to tell the program where those are
const char* WORLD_BUILDER_PROGRAM_UNIFORMS[] = {
    "tex0"
};

void loadWBProgram(){
    worldBuilderProgram = initProgram(VERT_SHADER_LOC, FRAG_SHADER_LOC);

    wbProgramUniforms = getUniformLocations(worldBuilderProgram, WORLD_BUILDER_PROGRAM_UNIFORMS);

    // Creating VAOs
    GLsizei numVAOs = 1;

    vaoArray = (GLuint *) malloc(sizeof(GLuint*) * numVAOs);
    glCreateVertexArrays(numVAOs, vaoArray);

    // Binding the rectangle VAO to the main program
    struct renderData* rect = initRect();
    bindVAO(rect, vaoArray[0], worldBuilderProgram);

    glUseProgram(worldBuilderProgram);

    // See if I want to keep texture loading here
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getTextures()[MONO1]);
    glUniform1i(wbProgramUniforms[WBP_TEX0], 0);
}

GLuint getWorldBuilderProgram(){
    if(!worldBuilderProgram){
        loadWBProgram();
    }

    return worldBuilderProgram;
}