#pragma once

//#include "Script.h"
#include "EntityManager.h"

class EntityManager;

class ScreenManager;

class Screen //: public Script
{
    //using Script::Script;

public:
    Screen(EntityManager* entityManager)
        : entityManager(entityManager) {}

    virtual ~Screen() = default;

    virtual void enter() = 0;
    virtual void exit() = 0;  
    //virtual void tickScript(float dt) = 0; 
    virtual bool getSwapToStartGame() {
        return swapToStartGame;
    }

protected:
    EntityManager* entityManager;
    ScreenManager* screenManager;

    bool swapToStartGame = false;
};