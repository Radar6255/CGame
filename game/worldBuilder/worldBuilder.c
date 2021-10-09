// File to contain all of the functions that tie into worldBuilder functionality, like the mouse handler

#include <limits.h>
#include <stdio.h>

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
    float relX = ((float) x) / (float) getWindowDims()[0];
    float relY = ((float) y) / (float) getWindowDims()[1];
    
    if (key == 'a'){
        addRenderObject(getWBRenderer(), relX, relY, getWBVAOArray(0));
    }
}