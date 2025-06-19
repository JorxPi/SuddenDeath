#pragma once

#include "OverlaySystem.h"

class InputSystem : public ECS::EntitySystem {
public:
    InputSystem(GLFWwindow* window, OverlaySystem* overlaySystem);

    virtual void tick(ECS::World* world, float dt) override;

private:
    GLFWwindow* window;
    OverlaySystem* overlaySystem;

    bool scoreOverlayActive = false; 
};