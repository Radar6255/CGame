#include "../worldBuilder/worldBuilder.h"
#include <stdio.h>

// Keyboard handler
void keyboard(unsigned char key, int x, int y){
    printf("Keyboard handling...\n");
    wbKeyHandler(key, x, y);
}