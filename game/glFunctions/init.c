#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <cglm/cglm.h>

#include "headers/init.h"
#include "headers/textureLoading.h"
#include "headers/texturesList.h"
#include "renderObjects/rectangle.h"
#include "renderObjects/renderData.h"
#include "../../header/parser.h"
#include "../engine/headers/camera.h"

#define VERT_SHADER_LOC "game/glFunctions/shaderCode/vertexShader.glsl"
#define FRAG_SHADER_LOC "game/glFunctions/shaderCode/fragShader.glsl"
#define GEO_SHADER_LOC "game/glFunctions/shaderCode/geometryShader.glsl"

struct ShaderCode{
    const GLchar ** code;
    GLint* codeLength;
};

// All of the uniforms that are in the main program
// Used to tell the program where those are
const char* MAIN_PROGRAM_UNIFORMS[] = {
    "tex0",
    "cameraPos",
    "screenTrans"
};

// Arrays and OpenGL values that get used through out the program

// The main program that does most if not all the rendering
static GLuint mainProgram = 0;
// An array of openGL program uniform locations
static GLuint* mainProgramUniforms;
// Array of openGL texture locations
static GLuint* textures;
// Array of openGL VAO's
static GLuint* vaoArray;

//static struct renderData *rect;

// Loads in the shader code from a file.
// Returns a pointer to a const char*, both of which should be freed
struct ShaderCode getShaderCode(char* filename){
    FILE *file = fopen(filename, "r");
    if (!file){
        printf("Unable to open the shader code file %s\n", filename);
        struct ShaderCode empty;
        empty.code = NULL;
        empty.codeLength = NULL;
        return empty;
    }

    // Creating a char* for the size of this file
    fseek(file, 0L, SEEK_END);
    int size = ftell(file) + 1;
    char* contents = (char *) malloc(sizeof(char) * size);
    fseek(file, 0L, SEEK_SET);

    for(int i = 0; i < size; i++){
        contents[i] = getc(file);
    }
    contents[size-1] = 0;

    struct ShaderCode out;
    out.code = (const GLchar**) &contents;
    out.codeLength = &size;

    fclose(file);
    return out;
}

// Shader code is the source code for this shader
// vertexShader is a bool that is 1 when the shader is a vertex shader 0 otherwise
GLuint createShader(const GLchar** shaderCode, GLint* shaderLength, GLenum shaderType){
    GLuint out = glCreateShader(shaderType);
    if(!out){
        printf("OpenGL failed to create a shader to use.\n");
        exit(12);
    }
    glShaderSource(out, 1, shaderCode, shaderLength);
    glCompileShader(out);

    GLint compileStatus;
    glGetShaderiv(out, GL_COMPILE_STATUS, &compileStatus);
    printf("Shader compile status: %s\n", compileStatus == GL_TRUE ? "Success" : "Failed");

    if(compileStatus != GL_TRUE) {
        GLsizei maxLength;
        glGetShaderiv(out, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* infoLog = (GLchar *) malloc(sizeof(GLchar) * maxLength);
        glGetShaderInfoLog(out, maxLength, NULL, infoLog);
        printf("%s\n", (char *) infoLog);
        free(infoLog);
        return 0;
    }

    return out;
}

// Initializes the shaders for the glProgram
GLuint initProgram(char* vertShaderLoc, char* geometryShaderLoc, char* fragShaderLoc){
    GLuint program = glCreateProgram();

    printf("\nLoading vertex shader...\n");
    struct ShaderCode source = getShaderCode(vertShaderLoc);
    GLuint vertShader = createShader(source.code, source.codeLength, GL_VERTEX_SHADER);
    free(*(GLchar**) source.code);

    printf("\nLoading geometry shader...\n");
    source = getShaderCode(geometryShaderLoc);
    GLuint geoShader = createShader(source.code, source.codeLength, GL_GEOMETRY_SHADER);
    free(*(GLchar**) source.code);

    printf("\nLoading fragment shader...\n");
    source = getShaderCode(fragShaderLoc);
    GLuint fragShader = createShader(source.code, source.codeLength, GL_FRAGMENT_SHADER);
    free(*(GLchar**) source.code);

    glAttachShader(program, vertShader);
    glAttachShader(program, geoShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);

    glDetachShader(program, vertShader);
    glDetachShader(program, geoShader);
    glDetachShader(program, fragShader);

    GLint params;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    printf("Program linked: %s\n", params == GL_TRUE ? "Success" : "Failed");
    if (params != GL_TRUE){
        GLint errorSize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorSize);

        GLchar* errorLog = (GLchar *) malloc(sizeof(GLchar) * errorSize);
        glGetProgramInfoLog(program, errorSize, NULL, errorLog);
        printf("%s\n", (char *) errorLog);
        free(errorLog);
        glDeleteProgram(program);

        glDeleteShader(vertShader);
        glDeleteShader(geoShader);
        glDeleteShader(fragShader);
    }

    return program;
}

GLuint* getUniformLocations(GLuint program, int numUniforms, const char** uniformNames){
    if (numUniforms){

    }
    GLuint* out = (GLuint*) malloc(numUniforms * sizeof(GLuint));
    glGetUniformIndices(program, numUniforms, uniformNames, out);
    printf("Initialized uniform 1 as %d\n", out[0]);
    return out;
}

// Sets up the projection matrix for the graphics calculations
// Needs to be called on every window update due to it needing the aspect ratio
void setProjMat(int windowWidth, int windowHeight){
    glUseProgram(mainProgram);

    mat4 projMat;
    glm_perspective(M_PI / 3, (float) windowWidth / windowHeight, 1, 200, projMat);
    glUniformMatrix4fv(MP_PROJ_MAT, 1, GL_FALSE, projMat[0]);
}

// Binds a VAO object to a program and also binds the buffers
void bindVAO(struct renderObject* data, GLuint vao, GLuint program){
    // Binding the vao to operate on
    glBindVertexArray(vao);

    GLuint *buffers = (GLuint *) malloc(sizeof(GLuint *) * RENDER_DATA_BUFFERS);
    glCreateBuffers(RENDER_DATA_BUFFERS, buffers);

    // Loading the data into the buffer
    glNamedBufferData(buffers[0], data->numVerts * sizeof(float), data->verticies, GL_STATIC_DRAW);

    // Bind the buffer to the VAO to use as verticies
    // The sizeof(float) * 3 may change if I decide to use a triangle strip instead of individual triangles
    glVertexArrayVertexBuffer(vao, 0, buffers[0], 0, sizeof(float) * 3);
    glVertexArrayAttribBinding(vao, 0, 0);

    // Telling OpenGL the format of our buffer
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayBindingDivisor(vao, 0, 0);

    // Enabling the vertex attribute in the vertex shader
    glEnableVertexArrayAttrib(vao, 0);

    if (data->texCoords){
        // Loading the texture coords into a buffer
        glNamedBufferData(buffers[1], data->numVerts * sizeof(float) * 2, data->texCoords, GL_STATIC_DRAW);

        // Bind the buffer to the VAO to use as texture coords
        glVertexArrayVertexBuffer(vao, 1, buffers[1], 0, sizeof(float) * 2);
        glVertexArrayAttribBinding(vao, 1, 1);

        // Telling OpenGL the format of our buffer
        glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayBindingDivisor(vao, 1, 0);

        // Enabling the texture attribute in the vertex shader
        glEnableVertexArrayAttrib(vao, 1);
    }

    // Freeing the buffers for now it may be wiser to keep this and do glDelete of the buffers later if possible
    free(buffers);
}

// Taken from https://www.khronos.org/opengl/wiki/OpenGL_Error
// Handles printing OpenGL errors to make debugging easier
void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "----OpenGL %s message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "status" ),
             message );
}

// Function to set up some OpenGL constants and set up the debug function for OpenGL
void initGL(){
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, NULL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);
}

// Loads the main program, its uniforms, and textures
void loadMainProgram(){
    printf("Loading main program\n");
    // Loads the shaders to the graphics card
    mainProgram = initProgram(VERT_SHADER_LOC, GEO_SHADER_LOC, FRAG_SHADER_LOC);
    // mainProgramUniforms = getUniformLocations(mainProgram, MP_NUM_UNIFORMS, MAIN_PROGRAM_UNIFORMS);
    // setUpCamera(windowWidth, windowHeight);

    textures = getTextures();

    GLsizei numVAOs = 1;
    // rect = initRect();
    // TODO Free the memory from this at some point
    struct renderObject model;
    // TODO Make it so that loadModel returns a renderObject struct instead of just the verticies
    model.verticies = loadModel("cube.obj", &model.numVerts);
    model.texCoords = NULL;
    model.normals = NULL;

    vaoArray = (GLuint *) malloc(sizeof(GLuint*) * numVAOs);
    // Creating VAOs
    glCreateVertexArrays(numVAOs, vaoArray);

    // Binding the rectangle VAO to the main program
    bindVAO(&model, vaoArray[0], mainProgram);
    printf("Binded VAO\n");

    glUseProgram(mainProgram);
    initCamera(MP_VIEW_MAT);

    // See if I want to keep texture loading here
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[MONO1]);
    // glUniform1i(mainProgramUniforms[MP_TEX0], 0);
}


// Declaring getters so that we can render things outside of this file

// Gets the main program for the main game
GLuint getMainProgram(){
    // If the main program hasn't been made yet initialize it
    if(!mainProgram){
        loadMainProgram();
    }

    return mainProgram;
}

GLuint getVAO(int index){
    return vaoArray[index];
}

GLuint uniformPosition(int index){
    return mainProgramUniforms[index];
}

// TODO Call this if the program was sucessfully initialized otherwise this may crash
void freeGLResources(){
    printf("\nClosing program...\n");
    // Freeing openGL stuff
    glDeleteProgram(mainProgram);

    // Free anything that was created in initialization
    free(vaoArray);
    free(textures);
    free(mainProgramUniforms);
    printf("Closed!\n");
}
