#include "../worldBuilder/worldBuilder.h"
#include "../main.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

void gameKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch(action){
        case GLFW_PRESS:
            switch(key){
                case GLFW_KEY_E:
                    printf("Testing keyhandling...\n");
                    break;
            }
            break;
    }
}

// Keyboard handler
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch (getMode())
    {
    case WBUILDER:
        // wbKeyHandler(key, x, y);
        break;
    case GAME:
        gameKeyboardHandler(window, key, scancode, action, mods);
        break;
    
    default:
        break;
    }
}
