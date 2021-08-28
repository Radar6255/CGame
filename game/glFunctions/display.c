#include "headers/init.h"
#include "../main.h"

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
    // setProjMat(w, h);
}

void display(void){
    // Getting the time we entered into the function
    struct timespec start;
    timespec_get(&start, TIME_UTC);
    if(!init){
        printf("Running OpenGL initialization\n");

        // This loads in some constant OpenGL settings for all programs
        // It also sets up OpenGL debug output
        initGL();

        // TODO See if needed
        switch (getMode())
        {
        case WBUILDER:
            
            // TODO Add another initialization function for world builder
            break;
        
        case GAME:
            break;
        }
        init = 1;
    }
    // TODO Update any rendering that needs to be done

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowWidth, windowHeight);

    switch (getMode()) {
    case WBUILDER:
        // TODO Call the code for the world builder here so that it doesn't need to change anything but main and display
        break;
    case GAME:
        // TODO Throw this in another file that will handle rendering for the game
        glUseProgram(getMainProgram());
        glBindVertexArray(getVAO(0));
        glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT);
        break;
    default:
        printf("Please specify a valid mode or add a display function to display.c\n");
        exit(900);
        break;
    }


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