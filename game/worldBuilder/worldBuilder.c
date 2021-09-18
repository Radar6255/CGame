// File to contain all of the functions that tie into worldBuilder functionality, like the mouse handler

#include <limits.h>
#include <stdio.h>

void wbMouseHandler(int button, int state, int x, int y){
    if(button != INT_MAX){
        // Got a click event

    }else{
        // Just a mouse movement event
        
    }
}

void wbKeyHandler(unsigned char key, int x, int y){
    printf("Pressed: %c\n", key);
    if (key == 'a'){
        
    }
}