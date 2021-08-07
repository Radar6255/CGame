#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cglm/cglm.h>

#include "headers/init.h"
#include "headers/textureLoading.h"
#include "headers/texturesList.h"
#include "renderObjects/rectangle.h"
#include "renderObjects/renderData.h"

#define VERT_SHADER_LOC "game/glFunctions/shaderCode/vertexShader.glsl"
#define FRAG_SHADER_LOC "game/glFunctions/shaderCode/fragShader.glsl"

struct ShaderCode{
    const GLchar ** code;
    GLint* codeLength;
};

// Holds the location of the uniform locations in MAIN_PROGRAM_UNIFORMS.
// Naming convention MP(Main Program), underscores for space and all caps
enum mainProgramUniforms{
    MP_TEMP_TEXTURE,
    MP_NUM_UNIFORMS
};

// All of the uniforms that are in the main program
// Used to tell the program where those are
const char* MAIN_PROGRAM_UNIFORMS[] = {
    "tempTexture"
};

// Arrays and OpenGL values that get used through out the program

// The main program that does most if not all the rendering
GLuint mainProgram;
// An array of openGL program uniform locations
GLuint* mainProgramUniforms;
// Array of openGL texture locations
GLuint* textures;
// Array of openGL VAO's
GLuint* vaoArray;

struct renderData *rect;

// Starting camera attributes
vec3 startCameraPos = {0, 0, 0};
vec3 startCameraDirection = {1.5, 0, 0};
vec3 cameraUp = {0, 1, 0};

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

    // GLchar* const* out = (GLchar* const*) malloc(sizeof(char *));

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
GLuint createShader(const GLchar** shaderCode, GLint* shaderLength, char vertexShader){
    GLuint out = glCreateShader(GL_VERTEX_SHADER * vertexShader + GL_FRAGMENT_SHADER * (1 - vertexShader));
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
GLuint initProgram(){
    GLuint program = glCreateProgram();

    printf("\nLoading vertex shader...\n");
    struct ShaderCode source = getShaderCode(VERT_SHADER_LOC);
    GLuint vertShader = createShader(source.code, source.codeLength, 1);
    free(*(GLchar**) source.code);

    printf("\nLoading fragment shader...\n");
    source = getShaderCode(FRAG_SHADER_LOC);
    GLuint fragShader = createShader(source.code, source.codeLength, 0);
    free(*(GLchar**) source.code);

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
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
    }

    return program;
}

GLuint* getUniformLocations(GLuint program, const char** uniformNames){
    GLuint* out = (GLuint*) malloc(MP_NUM_UNIFORMS * sizeof(GLuint));
    glGetUniformIndices(program, MP_NUM_UNIFORMS, uniformNames, out);
    return out;
}

// void setUpCamera(int windowWidth, int windowHeight){
//     setProjMat(windowWidth, windowHeight);

//     glUseProgram(mainProgram);
//     mat4 viewMat;
//     glm_lookat(startCameraPos, startCameraDirection, cameraUp, viewMat);
//     glUniformMatrix4fv(MP_VIEW_MAT, 1, GL_FALSE, viewMat[0]);
// }

// // Sets up the projection matrix for the graphics calculations
// // Needs to be called on every window update due to it needing the aspect ratio
// void setProjMat(int windowWidth, int windowHeight){
//     glUseProgram(mainProgram);

//     mat4 projMat;
//     glm_perspective(M_PI / 3, windowWidth / windowHeight, 1, 200, projMat);
//     glUniformMatrix4fv(MP_PROJ_MAT, 1, GL_FALSE, projMat[0]);
// }

// Binds a VAO object to a program and also binds the buffers
// TODO Make this also bind the texture coordinates
void bindVAO(struct renderData* data, GLuint vao, GLuint program){
    // Binding the vao to operate on
    glBindVertexArray(vao);

    GLuint *buffers = (GLuint *) malloc(sizeof(GLuint *) * RENDER_DATA_BUFFERS);
    glCreateBuffers(RENDER_DATA_BUFFERS, buffers);

    // Loading the data into the buffer
    glNamedBufferData(buffers[0], sizeof(data->points), data->points, GL_STATIC_DRAW);

    // Bind the buffer to the VAO to use as verticies
    glVertexArrayVertexBuffer(vao, 0, buffers[0], 0, sizeof(float) * 3);
    glVertexArrayAttribBinding(vao, 0, 0);

    // Telling OpenGL the format of our buffer
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayBindingDivisor(vao, 0, 0);

    // Enabling the vertex attribute in the vertex shader
    glEnableVertexArrayAttrib(vao, 0);

    // Loading the texture coords into a buffer
    glNamedBufferData(buffers[1], sizeof(data->texCoords), data->texCoords, GL_STATIC_DRAW);

    // Bind the buffer to the VAO to use as texture coords
    glVertexArrayVertexBuffer(vao, 1, buffers[1], 0, sizeof(float) * 2);
    glVertexArrayAttribBinding(vao, 1, 0);

    // Telling OpenGL the format of our buffer
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayBindingDivisor(vao, 1, 0);

    // Enabling the texture attribute in the vertex shader
    glEnableVertexArrayAttrib(vao, 1);

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
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void initGL(int windowWidth, int windowHeight){
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    // Loads the shaders to the graphics card
    mainProgram = initProgram();
    mainProgramUniforms = getUniformLocations(mainProgram, MAIN_PROGRAM_UNIFORMS);
    // setUpCamera(windowWidth, windowHeight);

    textures = initializeTextures();

    GLsizei numVAOs = 1;
    rect = initRect();

    vaoArray = (GLuint *) malloc(sizeof(GLuint*) * numVAOs);
    // Creating VAOs
    glCreateVertexArrays(numVAOs, vaoArray);

    // Binding the rectangle VAO to the main program
    bindVAO(rect, vaoArray[0], mainProgram);

    // TODO Figure out why the texture isn't showing up
    glUseProgram(mainProgram);
    // Temporary texture loading, just for testing need to find a good place for this
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[MONO1]);
    glUniform1i(mainProgramUniforms[MP_TEMP_TEXTURE], 0);
}


// Declaring getters so that we can render things outside of this file
GLuint getMainProgram(){
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
    printf("Closing program...\n");
    // Freeing openGL stuff
    glDeleteProgram(mainProgram);

    // Free anything that was created in initialization
    free(vaoArray);
    free(textures);
    free(mainProgramUniforms);
    printf("Closed!\n");
}