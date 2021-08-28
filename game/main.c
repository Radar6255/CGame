#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "glFunctions/headers/display.h"
#include "glFunctions/headers/keyboard.h"
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
enum RunningModes mode = GAME;

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
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(758, 568);

    glutInitWindowPosition(100, 100);
    
    switch (mode) {
    case GAME:
        glutCreateWindow("A Game in C");
        break;
    case WBUILDER:
        glutCreateWindow("World Builder Tool");
        break;
    default:
        glutCreateWindow("TODO Create a custom title");
        break;
    }
    
    printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));

    GLenum glewErr = glewInit();
    
    if(glewErr != GLEW_OK){
        printf("GLEW Error: %s\n", glewGetErrorString(glewErr));
        return 100;
    }
    printf("Successfully initialized GLEW.\n");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    atexit(freeGLResources);

    // Starts the main program
    glutMainLoop();
    return 0;
}