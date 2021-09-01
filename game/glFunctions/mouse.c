#include "../main.h"
#include "../worldBuilder/worldBuilder.h"

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <limits.h>


// This takes in the x,y position of the cursor and potentially the button pressed/released and whether it was released or pressed
// If the mouse state hasn't changed then button and state are INT_MAX to indicate no change
void mouseHandler(int button, int state, int x, int y){
    switch (getMode()) {
    case WBUILDER:
        wbMouseHandler(button, state, x, y);
        break;
    case GAME:
        // TODO Implement
        break;

    default:
        printf("TODO, implement keyhandler for current state or just a null case...\n");
        break;
    }
}

// Called when the mouse is clicked on the window
void mouseClick(int button, int state, int x, int y){
    printf("Clicked at %d, %d, %d\n", x, y, button);
    mouseHandler(button, state, x, y);
    
}

// Called when the mouse is just moved
void mouseMove(int x, int y){
    mouseHandler(INT_MAX, INT_MAX, x, y);
}

// Called when the mouse is moved and a button is pressed
void mouseClickMove(int x, int y){
    mouseHandler(INT_MAX, INT_MAX, x, y);
}