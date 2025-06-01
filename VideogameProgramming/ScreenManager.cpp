#include "ScreenManager.h"

ScreenManager::ScreenManager(GLFWwindow* window, ECS::World* world)
    : window(window), world(world) {

}

void ScreenManager::addScreen(const std::string& name, Screen* screen) {
    screens[name] = std::unique_ptr<Screen>(screen);
}

void ScreenManager::switchToScreen(std::string name) {
    if (currentScreen)
        currentScreen->exit();
    auto new_screen = screens.find(name);
    if (new_screen != screens.end()) {
        currentScreen = new_screen->second.get();
        currentScreen->enter();

        if (name == "StartGame") {
            gameManager->activate(true);
        }
        else {
            gameManager->activate(false);
        }
    } else {
        std::cerr << "ScreenManager::switchToScreen(): no existe la pantalla '" << name << "'\n";
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

void ScreenManager::update(float dt) {
    if (!currentScreen) return;
    // Comprobamos si la pantalla actual solicitó cambiar a otra (getRequestedSwap() != "")
    const std::string& next = currentScreen->getRequestedSwap();
    if (!next.empty()) {
        switchToScreen(next);
    }
}

void ScreenManager::setManagerRefs(GameManager* gmr, OverlayManager* omr) {
    gameManager = gmr;
    overlayManager = omr;
}

OverlayManager* ScreenManager::getOverlayManager() { 
    return overlayManager; 
}