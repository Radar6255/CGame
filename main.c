#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "glFunctions/display.h"
#include "glFunctions/keyboard.h"

void printErr(char* errMsg){
    fprintf(stderr, errMsg);
    getchar();
}


int main(int argc, char** argv){
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(758, 568);

    glutInitWindowPosition(100, 100);
    glutCreateWindow("A window");

    printf("Using OpenGL Version: %s\n", glGetString(GL_VERSION));

    GLenum glewErr = glewInit();
    
    if(glewErr != GLEW_OK){
        printf("GLEW Error: %s\n", glewGetErrorString(glewErr));
        return 100;
    }
    printf("Successfully initialized GLEW.\n");

    // Do init here
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    // Starts the main program
    glutMainLoop();
    return 0;
}