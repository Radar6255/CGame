#include <GLFW/glfw3.h>
#include "../engine/headers/camera.h"

// Called when the mouse is clicked on the window
void mouseClickE(GLFWwindow* window, int button, int action, int mods) {

}

// Called when the mouse is just moved
void mouseMoveE(GLFWwindow* window, double x, double y) {
    setCameraRotation(-x / 1000.0, -y / 1000.0);
}
