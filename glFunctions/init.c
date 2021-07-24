#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <cglm/cglm.h>

#define VERT_SHADER_LOC "glFunctions/shaderCode/vertexShader.glsl"
#define FRAG_SHADER_LOC "glFunctions/shaderCode/fragShader.glsl"

struct ShaderCode{
    const GLchar ** code;
    GLint* codeLength;
};

// Holds the location of the uniform locations in MAIN_PROGRAM_UNIFORMS.
// Naming convention MP(Main Program), underscores for space and all caps
enum mainProgram{
    MP_PROJ_MAT,
    MP_NUM_UNIFORMS
};

const char* MAIN_PROGRAM_UNIFORMS[] = {
    "projMat"
};

GLuint mainProgram;
GLuint* mainProgramUniforms;


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

void setUpCamera(int windowWidth, int windowHeight, GLuint program){
    glUseProgram(program);

    mat4 projMat;
    glm_perspective(M_PI / 3, windowWidth / windowHeight, 1, 200, projMat);
    glUniformMatrix4fv(MP_PROJ_MAT, 1, GL_FALSE, projMat[0]);
}

void initGL(int windowWidth, int windowHeight){
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    // Loads the shaders to the graphics card
    // TODO set this function active at some point
    mainProgram = initProgram();
    mainProgramUniforms = getUniformLocations(mainProgram, MAIN_PROGRAM_UNIFORMS);
    setUpCamera(windowWidth, windowHeight, mainProgram);

    //TODO Set up texture here
}

void freeGLResources(){
    // Free anything that was created in initialization
    free(mainProgramUniforms);
}