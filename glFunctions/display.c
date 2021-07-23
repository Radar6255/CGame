#include "init.h"
#include <stdio.h>
#include <GL/glew.h>

char init = 0;
int windowWidth, windowHeight;

// Called when the window is reshaped
void reshape(int w, int h){
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, windowWidth, windowHeight);
}

// Believe this is the main draw call
void display(void){
    printf("Running OpenGL initialization\n");
    initGL();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowWidth, windowHeight);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Call for another draw
    glutPostRedisplay();
}