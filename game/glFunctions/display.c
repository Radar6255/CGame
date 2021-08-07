#include "headers/init.h"

#include <stdio.h>
#include <time.h>

#include <GL/glew.h>
#include <GL/glut.h>

char init = 0;
int windowWidth, windowHeight;
int maxFramerate = 144;

// Called when the window is reshaped
void reshape(int w, int h){
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, windowWidth, windowHeight);
    // TODO Find if this needs to have a mutex because threading
    setProjMat(w, h);
}

void display(void){
    // Getting the time we entered into the function
    struct timespec start;
    timespec_get(&start, TIME_UTC);
    if(!init){
        printf("Running OpenGL initialization\n");
        initGL(windowWidth, windowHeight);
        init = 1;
    }
    // TODO Update any rendering that needs to be done

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowWidth, windowHeight);

    glUseProgram(getMainProgram());
    glBindVertexArray(getVAO(0));
    glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT);

    glutSwapBuffers();
    // Find out how long to wait before trying to call for another frame
    struct timespec end;
    timespec_get(&end, TIME_UTC);

    end.tv_sec = 0;
    end.tv_nsec = (long)  (1000000000 / maxFramerate) - (end.tv_nsec - start.tv_nsec);
    nanosleep(&end, &end);
    
    // Call for another draw
    glutPostRedisplay();
}