#include "../worldBuilder/worldBuilder.h"
#include "../main.h"

// Keyboard handler
void keyboard(unsigned char key, int x, int y){
    switch (getMode())
    {
    case WBUILDER:
        wbKeyHandler(key, x, y);
        break;
    case GAME:
        // TODO make the keyboard handler for this
        break;
    
    default:
        break;
    }
}