#include "ScreenManager.h"

ScreenManager::ScreenManager(GLFWwindow* window, ECS::World* world)
    : window(window), world(world) {

}

void ScreenManager::addScreen(const std::string& name, Screen* screen) {
    screens[name] = std::unique_ptr<Screen>(screen);
}

void ScreenManager::switchToScreen(const std::string& name) {
    if (currentScreen)
        currentScreen->exit();
    auto new_screen = screens.find(name);
    if (new_screen != screens.end()) {
        currentScreen = new_screen->second.get();
        currentScreen->enter();

        swappingScreen = false;

        if (name == "StartGame") {
            gameManager->activate(true);
        }
        else {
            gameManager->activate(false);
        }
    }
}

void ScreenManager::activateOverlay(bool state) {
    if (overlayManager) {
        overlayManager->activate(state);
    }
}

Screen* ScreenManager::getCurrentScreen() {
    return currentScreen;
}

//Probably not needed
void ScreenManager::update(float dt) {
    if (currentScreen->getSwapToStartGame()) {
        swappingScreen = true;
        switchToScreen("StartGame");
    }
}

void ScreenManager::setManagerRefs(GameManager* gmr, OverlayManager* omr) {
    gameManager = gmr;
    overlayManager = omr;
}

OverlayManager* ScreenManager::getOverlayManager() { 
    return overlayManager; 
}