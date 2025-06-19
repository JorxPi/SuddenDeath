#include "ScreenSystem.h"

ScreenSystem::ScreenSystem(GLFWwindow* window, ECS::World* world)
    : window(window), world(world) {

}

void ScreenSystem::tick(ECS::World* world, float dt) {
    if (!currentScreen) return;
    // Comprobamos si la pantalla actual solicitó cambiar a otra (getRequestedSwap() != "")
    const std::string& next = currentScreen->getRequestedSwap();
    if (!next.empty()) {
        switchToScreen(next);
    }
}

void ScreenSystem::addScreen(const std::string& name, Screen* screen) {
    screens[name] = std::unique_ptr<Screen>(screen);
}

void ScreenSystem::switchToScreen(std::string name) {
    if (currentScreen)
        currentScreen->exit();
    auto new_screen = screens.find(name);
    if (new_screen != screens.end()) {
        currentScreen = new_screen->second.get();
        currentScreen->enter();

        if (name == "StartGame") {
            gameManager->resetGame();
            gameManager->activate(true);
        }
        else {
            gameManager->activate(false);
        }
    } else {
        std::cerr << "ScreenSystem::switchToScreen(): no existe la pantalla '" << name << "'\n";
    }
}

void ScreenSystem::activateOverlay(const std::string& groupName, bool show) {
    if (!overlaySystem) return;

    if (show) {
        overlaySystem->showOverlayGroup(groupName);
    }
    else {
        overlaySystem->hideOverlayGroup(groupName);
    }
}

Screen* ScreenSystem::getCurrentScreen() {
    return currentScreen;
}

void ScreenSystem::setManagerRefs(GameManager* gmr, OverlaySystem* omr) {
    gameManager = gmr;
    overlaySystem = omr;
}

OverlaySystem* ScreenSystem::getOverlaySystem() {
    return overlaySystem;
}