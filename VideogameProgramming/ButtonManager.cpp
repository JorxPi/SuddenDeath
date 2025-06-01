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


//BUTTON UNDO


void ButtonManager::moveUndoButtonIn(InvocationScript* invoc) {
    if (undoButtonIn) return; // If already visible, do nothing

    if (!undoButtonRef) return; // Safety check

    undoTarget = invoc;
    ComponentHandle<Transform> transform = undoButtonRef->getEntity()->get<Transform>();
    transform->position = glm::vec2(200, 250); // Move to visible position
    undoButtonIn = true;
}

void ButtonManager::moveUndoButtonOut() {
    if (!undoButtonIn) return;

    ComponentHandle<Transform> transform = undoButtonRef->getEntity()->get<Transform>();
    transform->position = glm::vec2(-1000, -1000); // Move out of screen
    undoButtonIn = false;
}

void ButtonManager::handleUndoButtonClick() {
    if (undoTarget) {
        undoTarget->undoLastMove();
        moveUndoButtonOut();
    }
}



//BUTTON GAME OVER

void ButtonManager::moveGameOverButtonIn() {
    if (!gameOverButtonRef) return;
    Entity* e = gameOverButtonRef->getEntity();
    if (!e) return;
    ComponentHandle<Transform> t = e->get<Transform>();
    t->position = ButtonPos::gameOverButtonIn;
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

void ButtonManager::setUndoButtonRef(ButtonScript* button) {
    undoButtonRef = button;
}

void ButtonManager::setGameOverButtonRef(ButtonScript* button) {
    gameOverButtonRef = button;
}


