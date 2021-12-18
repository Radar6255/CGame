#include "../glFunctions/headers/init.h"
#include "../glFunctions/renderObjects/rectangle.h"
#include "../glFunctions/headers/textureLoading.h"
#include "../glFunctions/headers/texturesList.h"
#include "../engine/headers/render.h"
#include "init.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <stdlib.h>
#include <stdio.h>

static GLuint worldBuilderProgram;
static GLuint* wbProgramUniforms;
static GLuint* vaoArray;
static void* renderer = NULL;

#define VERT_SHADER_LOC "game/glFunctions/shaderCode/worldBuilder/vertexShader.glsl"
#define GEO_SHADER_LOC "game/glFunctions/shaderCode/worldBuilder/geometryShader.glsl"
#define FRAG_SHADER_LOC "game/glFunctions/shaderCode/worldBuilder/fragShader.glsl"

// All of the uniforms that are in the world builder program
// Used to tell the program where those are
const char* WORLD_BUILDER_PROGRAM_UNIFORMS[] = {
    "tex0",
    "screenTransform"
};

// This function gets the renderer that we will use for the World Builder.
// The renderer is actually made on the first call but that should only be a little overhead.
// Potentially move that out at some point
void* getWBRenderer(){
    if (!renderer){
        printf("Initializing renderer...\n");
        renderer = intializeRenderer(10);
    }
    
    return renderer;
}

void loadWBProgram(){
    worldBuilderProgram = initProgram(VERT_SHADER_LOC, GEO_SHADER_LOC, FRAG_SHADER_LOC);

    wbProgramUniforms = getUniformLocations(worldBuilderProgram, WBP_NUM_UNIFORMS, WORLD_BUILDER_PROGRAM_UNIFORMS);

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

GLuint getWBVAOArray(int index){
    return vaoArray[index];
}

// Returns the OpenGL location of the given index of a uniform
// The meaningful indicies for this array are from the worldBuilderProgramUniforms enum
GLuint getWBUniformLoc(int index){
    return wbProgramUniforms[index];
}

GLuint getWorldBuilderProgram(){
    return worldBuilderProgram;
}