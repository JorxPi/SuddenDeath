#pragma once

#include<iostream>
#include<string>
#include<map>
#include "ECS.h"
#include "Components.h"
#include "Texture.h"
#include "ScriptManager.h"

using namespace std;
using namespace ECS;

class ScriptSystem : public EntitySystem
{
public:

    ScriptSystem();
    virtual void tick(World* world, float deltaTime) override;
    virtual void start(World* world);

    ScriptManager* getScriptManager();

private:
    ScriptManager scriptManager;

};