#pragma once

#include "OverlayManager.h"

class InputManager {
public:
    InputManager(GLFWwindow* window, OverlayManager* overlayManager);

    void update(float deltaTime);

private:
    GLFWwindow* window;
    OverlayManager* overlayManager;

    bool scoreOverlayActive = false; 
};