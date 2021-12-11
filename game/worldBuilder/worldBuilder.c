// File to contain all of the functions that tie into worldBuilder functionality, like the mouse handler

#include <limits.h>
#include <stdio.h>
#include <time.h>

#include "init.h"
#include "../glFunctions/headers/display.h"
#include "../engine/headers/render.h"

void wbMouseHandler(int button, int state, int x, int y){
    if(button != INT_MAX){
        // Got a click event
        
    }else{
        // Just a mouse movement event
        
    }
}

void wbKeyHandler(unsigned char key, int x, int y){
    printf("Pressed: %c\n", key);
    // The y axis is flipped because y increases as you go down
    float relX = 2 * x / (float) getWindowDims()[0] - 1;
    float relY = -2 * y / (float) getWindowDims()[1] + 1;

    if (key == 'a'){
        time_t start = time(NULL);
        addRenderObject(getWBRenderer(), relX, relY, getWBVAOArray(0));

        printf("Took %ld seconds to add object\n", time(NULL) - start);
    }
}