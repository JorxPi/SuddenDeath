#include "InputManager.h"
#include <GLFW/glfw3.h>

InputManager::InputManager(GLFWwindow* window, OverlayManager* overlayManager)
    : window(window), overlayManager(overlayManager) {
}

void InputManager::update(float deltaTime) {
    // Handle the TAB key for showing/hiding the score overlay
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !scoreOverlayActive) {
        std::cout << "TAB pressed - Activating Score Overlay" << std::endl;
        overlayManager->activateScoreOverlay(true); // Show the score overlay
        scoreOverlayActive = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE && scoreOverlayActive) {
        std::cout << "TAB released - Deactivating Score Overlay" << std::endl;
        overlayManager->activateScoreOverlay(false); // Hide the score overlay
        scoreOverlayActive = false;
    }
}