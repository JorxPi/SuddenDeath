#include "ButtonManager.h"


ButtonManager::ButtonManager(GLFWwindow* window, ECS::World* world)
    : window(window), world(world) {
    
}

//BUTTON MOVING



void ButtonManager::moveSkipButtonIn() {
    Entity* skipButtonEnt = skipButtonRef->getEntity();
    ComponentHandle<Transform> transform = skipButtonEnt->get<Transform>();

    transform->position = ButtonPos::skipButtonIn;
}

void ButtonManager::moveSkipButtonOut() {
    Entity* skipButtonEnt = skipButtonRef->getEntity();
    ComponentHandle<Transform> transform = skipButtonEnt->get<Transform>();

    transform->position = ButtonPos::generalButtonOut;
}



//GETTERS



bool ButtonManager::getSkipTurnClicked() {
    return skipTurnClicked;
}



//SETTERS

void ButtonManager::setSkipButtonRef(ButtonScript* buttonScript) {
    skipButtonRef = buttonScript;
}

void ButtonManager::setSkipTurnClicked(bool value) {
    skipTurnClicked = value;
}


