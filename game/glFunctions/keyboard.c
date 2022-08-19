#include "../worldBuilder/worldBuilder.h"
#include "../main.h"
#include "../engine/headers/camera.h"

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void gameKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods){
    vec3 left = {0, 0, -0.1};
    vec3 right = {0, 0, 0.1};
    vec3 up = {0, 0.1, 0};
    vec3 down = {0, -0.1, 0};
    vec3 forward = {0.1, 0, 0};
    vec3 back = {-0.1, 0, 0};

    vec3 out = {0, 0, 0};

    switch(action){
        case GLFW_PRESS:
            switch(key){
                case GLFW_KEY_A:
                    glm_vec3_add(left, out, out);
                    break;
                case GLFW_KEY_D:
                    glm_vec3_add(right, out, out);
                    break;
                case GLFW_KEY_S:
                    glm_vec3_add(back, out, out);
                    break;
                case GLFW_KEY_W:
                    glm_vec3_add(forward, out, out);
                    break;
                case GLFW_KEY_SPACE:
                    glm_vec3_add(up, out, out);
                    break;
                case GLFW_KEY_C:
                    glm_vec3_add(down, out, out);
                    break;
            }
            break;
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
