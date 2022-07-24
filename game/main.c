#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glFunctions/headers/display.h"
#include "glFunctions/headers/keyboard.h"
#include "glFunctions/headers/mouse.h"
#include "glFunctions/headers/init.h"

enum RunningModes{
  GAME,
  WBUILDER
};

void printErr(char* errMsg){
    fprintf(stderr, errMsg);
    getchar();
}

// Used to keep what the engine is running at the moment
static enum RunningModes mode = GAME;

enum RunningModes getMode(){
    return mode;
}

int main(int argc, char** argv){

    if (argc > 1){
        // TODO Possibly move this into another function if I need to do a lot of specific things for the world builder
        if (!strcmp(argv[1], "wbuilder")){
            printf("Starting world builder...\n");
            mode = WBUILDER;
        }else if(!strcmp(argv[1], "-w")){
            printf("Starting world builder...\n");
            mode = WBUILDER;
        }
    }
    GLFWwindow* window;

    if (!glfwInit()){
        printErr("Unable to initialize GLFW!\n");
        return 100;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    char* title;
    switch (mode) {
    case GAME:
        title = "A Game in C";
        break;
    case WBUILDER:
        title = "World Builder Tool";
        break;
    default:
        title = "TODO Create a custom title";
        break;
    }

    /* glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); */
    window = glfwCreateWindow(758, 758, title, NULL, NULL);
    if (!window){
        glfwTerminate();
        printErr("Unable to create window!\n");
        return 100;
    }

    glfwMakeContextCurrent(window);

    printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));

    GLenum glewErr = glewInit();

    if(glewErr != GLEW_OK){
        printf("GLEW Error: %s\n", glewGetErrorString(glewErr));
        return 100;
    }
    printf("Successfully initialized GLEW.\n");

    glfwSetWindowSizeCallback(window, reshape);

    while(!glfwWindowShouldClose(window)){
        display(window);

        glfwPollEvents();
    }

    /* glutKeyboardFunc(keyboard); */
    /* glutMouseFunc(mouseClick); */
    /* glutMotionFunc(mouseClickMove); */
    /* glutPassiveMotionFunc(mouseMove); */
    atexit(freeGLResources);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
