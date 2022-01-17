#include "headers/init.h"
#include "../main.h"
#include "../worldBuilder/worldBuilder.h"
#include "../worldBuilder/init.h"
#include "../engine/headers/render.h"
#include "../worldBuilder/init.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <GL/glew.h>

char init = 0;
int *windowDimensions = NULL;

// Called when the window is reshaped
void reshape(int w, int h){
    if (!windowDimensions){
        windowDimensions = malloc(sizeof(int) * 2);
    }
    windowDimensions[0] = w;
    windowDimensions[1] = h;
    glViewport(0, 0, w, h);
    printf("Got a resize of %d, %d\n", w, h);
    // TODO Find if this needs to have a mutex because threading
    // setProjMat(w, h);
}

const int* getWindowDims(){
    return windowDimensions;
}

void display(){
    if (!init){
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
        glDrawArrays(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT);
        break;
    default:
        printf("Please specify a valid mode or add a display function to display.c\n");
        exit(900);
        break;
    }
}