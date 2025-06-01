#pragma once

#include "ScriptManager.h"
#include "ScriptSystem.h"
#include "GameManager.h"
#include "OverlayManager.h"
#include <memory>
#include <map>
#include <string>
#include "Screen.h"

class Screen;

class OverlayManager;

class ScreenManager {
public:
	ScreenManager(GLFWwindow* window, ECS::World* world);

	void addScreen(const std::string& name, Screen* screen);
	void switchToScreen(std::string name);
	void activateOverlay(bool state);

	Screen* getCurrentScreen();

	void update(float dt);

	void setManagerRefs(GameManager* gmr, OverlayManager* omr);

	OverlayManager* getOverlayManager();

private:
	GLFWwindow* window;
	ECS::World* world;

	GameManager* gameManager;
	OverlayManager* overlayManager;

	std::map<std::string, std::unique_ptr<Screen>> screens;
	Screen* currentScreen = nullptr;

};
