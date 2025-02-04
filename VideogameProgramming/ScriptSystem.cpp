#include "ScriptSystem.h"

ScriptSystem::ScriptSystem() {
}

void ScriptSystem::tick(World* world, float deltaTime)
{
    world->each<ScriptComponent>([&](Entity* ent, ComponentHandle<ScriptComponent> script) {

        scriptManager.tickScript(script->scriptId, deltaTime);

    });
}

void ScriptSystem::start(World* world)
{
    world->each<ScriptComponent>([&](Entity* ent, ComponentHandle<ScriptComponent> script) {

        scriptManager.startScript(script->scriptId);

    });
}

ScriptManager* ScriptSystem::getScriptManager() {

    return &scriptManager;

}