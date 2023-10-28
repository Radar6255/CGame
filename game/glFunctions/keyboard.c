#include "../worldBuilder/worldBuilder.h"
#include "../main.h"
#include "../engine/headers/camera.h"
#include "./headers/keyboard.h"

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

bool left = false;
bool right = false;
bool up = false;
bool down = false;
bool forward = false;
bool back = false;

void gameKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch(action){
        case GLFW_PRESS:
            switch(key){
                case GLFW_KEY_A:
                    left = true;
                    break;
                case GLFW_KEY_D:
                    right = true;
                    break;
                case GLFW_KEY_S:
                    back = true;
                    break;
                case GLFW_KEY_W:
                    forward = true;
                    break;
                case GLFW_KEY_SPACE:
                    up = true;
                    break;
                case GLFW_KEY_C:
                    down = true;
                    break;
            }
            break;
        case GLFW_RELEASE:
            switch(key){
                case GLFW_KEY_A:
                    left = false;
                    break;
                case GLFW_KEY_D:
                    right = false;
                    break;
                case GLFW_KEY_S:
                    back = false;
                    break;
                case GLFW_KEY_W:
                    forward = false;
                    break;
                case GLFW_KEY_SPACE:
                    up = false;
                    break;
                case GLFW_KEY_C:
                    down = false;
                    break;
            }
            break;
    }
    performMovementUpdate();
}

void performMovementUpdate(){
    vec3 leftM = {0, 0, -0.1};
    vec3 rightM = {0, 0, 0.1};
    vec3 upM = {0, 0.1, 0};
    vec3 downM = {0, -0.1, 0};
    vec3 forwardM = {0.1, 0, 0};
    vec3 backM = {-0.1, 0, 0};

    vec3 out = {0, 0, 0};

    if (left){
        glm_vec3_add(leftM, out, out);
    } if (right){
        glm_vec3_add(rightM, out, out);
    } if (back){
        glm_vec3_add(backM, out, out);
    } if (forward){
        glm_vec3_add(forwardM, out, out);
    } if (up){
        glm_vec3_add(upM, out, out);
    } if (down){
        glm_vec3_add(downM, out, out);
    }

    moveCamera(out);
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
