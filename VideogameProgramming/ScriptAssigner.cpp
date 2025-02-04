#include "ScriptAssigner.h"
#include "InvocationScript.h"
#include "CellScript.h"
#include "CoinScript.h"
#include "RuletaAttacks.h"
#include "RuletaBatalla.h"
#include "ButtonScript.h"
#include "InvocationSelectionScript.h"

Script* ScriptAssigner::createScript(const std::string& scriptName, ECS::Entity* entity, GLFWwindow* window, ECS::World* world) {
    if (scriptName == "InvocationScript") {
        return new InvocationScript(window, world, entity);
    }
    if (scriptName == "CellScript") {
        return new CellScript(window, world, entity);
    }
    if (scriptName == "CoinScript") {
        return new CoinScript(window, world, entity);
    }
    if (scriptName == "RuletaAttacks") {
        return new RuletaAttacks(window, world, entity);
    }
    if (scriptName == "RuletaBatalla") {
        return new RuletaBatalla(window, world, entity);
    }
    if (scriptName == "ButtonScript") {
        return new ButtonScript(window, world, entity);
    }
    if (scriptName == "InvocationSelectionScript") {
        return new InvocationSelectionScript(window, world, entity);
    }
    return nullptr;
}