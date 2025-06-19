#pragma once

#include "ScriptManager.h"
#include "ScriptSystem.h"
#include "GameManager.h"
#include "OverlaySystem.h"
#include <memory>
#include <map>
#include <string>
#include "Screen.h"
#include "ECS.h"

class Screen;

class OverlaySystem;

class ScreenSystem : public ECS::EntitySystem {
public: 
	ScreenSystem(GLFWwindow* window, ECS::World* world);

	virtual void tick(ECS::World* world, float dt) override;

	void addScreen(const std::string& name, Screen* screen);
	void switchToScreen(std::string name);
	void activateOverlay(const std::string& groupName, bool show);

	Screen* getCurrentScreen();

	void setManagerRefs(GameManager* gmr, OverlaySystem* omr);

	OverlaySystem* getOverlaySystem();

private:
	GLFWwindow* window;
	ECS::World* world;

	GameManager* gameManager;
	OverlaySystem* overlaySystem;

	std::map<std::string, std::unique_ptr<Screen>> screens;
	Screen* currentScreen = nullptr;

};
