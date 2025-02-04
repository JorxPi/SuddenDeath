#pragma once

#include <string>
#include "ScriptManager.h"
#include "ScriptSystem.h"


class ScriptAssigner {

public:
	static Script* createScript(const std::string& scriptName, ECS::Entity* entity, GLFWwindow* window, ECS::World* world);

private:

};