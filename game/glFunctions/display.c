#include "headers/init.h"
#include "../main.h"
#include "../worldBuilder/worldBuilder.h"
#include "../worldBuilder/init.h"
#include "../engine/headers/render.h"
#include "../worldBuilder/init.h"

#include <stdio.h>
#include <time.h>
#include <GLFW/glfw3.h>

#include <GL/glew.h>

char init = 0;
int *windowDimensions = NULL;
int maxFramerate = 144;

// Called when the window is reshaped
void reshape(GLFWwindow* window, int w, int h){
    if (!windowDimensions){
        windowDimensions = malloc(sizeof(int) * 2);
    }
    windowDimensions[0] = w;
    windowDimensions[1] = h;
    glViewport(0, 0, w, h);

    setProjMat(w, h);
}

const int* getWindowDims(){
    return windowDimensions;
}

void display(GLFWwindow* window){
    // Getting the time we entered into the function
    // struct timespec start;
    // timespec_get(&start, TIME_UTC);
    if(!init){
        printf("Running OpenGL initialization\n");

        // This loads in some constant OpenGL settings for all programs
        // It also sets up OpenGL debug output
        initGL();

        // TODO See if needed
        switch (getMode()) {
        case WBUILDER:
            // TODO Add another initialization function for world builder
            loadWBProgram();
            break;

        case GAME:
            break;
        }
        init = 1;
    }
    // TODO Update any rendering that needs to be done

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Finding if we have the current window dimensions already
    if (!windowDimensions){
        // If we don't then get those
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        reshape(window, width, height);
    }

    glViewport(0, 0, windowDimensions[0], windowDimensions[1]);

    switch (getMode()) {
    case WBUILDER:
        // TODO Consider making the renderer remember all of these arguments
        // Probably a little bit faster than chucking it on the stack then taking it off again every frame
        // Doesn't really matter too much though
        renderAllObjects(getWBRenderer(), getWorldBuilderProgram(), getWBUniformLoc(WBP_WORLD_T));
        break;
    case GAME:
        // TODO Throw this in another file that will handle rendering for the game
        glUseProgram(getMainProgram());
        glBindVertexArray(getVAO(0));
        // TODO Need to bind the world and screen transforms here most likely
        glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT);
        break;
    default:
        printf("Please specify a valid mode or add a display function to display.c\n");
        exit(900);
        break;
    }

    glfwSwapBuffers(window);
    // Find out how long to wait before trying to call for another frame
    // struct timespec end;
    // timespec_get(&end, TIME_UTC);

    // end.tv_sec = 0;
    // end.tv_nsec = (long)  (1000000000 / maxFramerate) - (end.tv_nsec - start.tv_nsec);
    // nanosleep(&end, &end);
}
