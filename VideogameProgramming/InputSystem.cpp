#include "InputSystem.h"
#include <GLFW/glfw3.h>

InputSystem::InputSystem(GLFWwindow* window, OverlaySystem* overlaySystem)
    : window(window), overlaySystem(overlaySystem) {
}

void InputSystem::tick(ECS::World* world, float dt) {
    // Handle the TAB key for showing/hiding the score overlay
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !scoreOverlayActive) {
        std::cout << "TAB pressed - Activating Score Overlay" << std::endl;
        //overlaySystem->activateScoreOverlay(true); // Show the score overlay
        //scoreOverlayActive = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE && scoreOverlayActive) {
        std::cout << "TAB released - Deactivating Score Overlay" << std::endl;
        //overlaySystem->activateScoreOverlay(false); // Hide the score overlay
        //scoreOverlayActive = false;
    }
}