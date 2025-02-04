#pragma once

#include "ScriptManager.h"
#include "ScriptSystem.h"
#include "ButtonScript.h"
#include "InvocationScript.h"
#include "ButtonConstants.h"
#include <functional>
#include <vector>

class ButtonScript;

class InvocationScript;

class ButtonManager {

public:
	ButtonManager(GLFWwindow* window, ECS::World* world);

	//BUTTON MOVING
	void moveSkipButtonIn();
	void moveSkipButtonOut();

	//GETTERS
	bool getSkipTurnClicked();

	//SETTERS
	void setSkipButtonRef(ButtonScript* buttonScript);
	void setSkipTurnClicked(bool value);


private: 	
	GLFWwindow* window;
	ECS::World* world;

	ButtonScript* skipButtonRef;

	bool skipTurnClicked = false;

};