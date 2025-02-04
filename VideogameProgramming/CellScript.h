#pragma once

#include "Script.h"
#include "InvocationScript.h"

class InvocationScript;

class CellScript : public Script
{
    using Script::Script;

public:
    void tickScript(float deltaTime) override;

    //GENERAL LOGIC
    bool cellToMove(float deltaTime, std::vector<Entity*> cellsToMoveList, InvocationScript* invocationScript);

    //GETTERS
    float getPosX();
    float getPosY();
    Entity* getCellEntity();

    //SETTERS   
    void setCellRef(Entity* cellr);
    

private:
    Entity* cellEnt;
};