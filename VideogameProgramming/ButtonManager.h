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

	//BUTTON UNDO
	void moveUndoButtonIn(InvocationScript* invoc);
	void moveUndoButtonOut();
	void handleUndoButtonClick();

	//GETTERS
	bool getSkipTurnClicked();

	//SETTERS
	void setSkipButtonRef(ButtonScript* buttonScript);
	void setSkipTurnClicked(bool value);
	void setUndoButtonRef(ButtonScript* button);


private: 	
	GLFWwindow* window;
	ECS::World* world;

	ButtonScript* skipButtonRef;
	ButtonScript* undoButtonRef = nullptr;
	InvocationScript* undoTarget = nullptr;
	bool undoButtonIn = false;

	bool skipTurnClicked = false;

};