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

    bool getIsConquerable();
    int getOwner();

    //SETTERS   
    void setCellRef(Entity* cellr);

    void setOwner(int newOwner);
    void setConquerable(bool conquerable);
    

private:
    Entity* cellEnt;

    bool isConquerable = false;  // Default: not conquerable
    int owner = 0;  // 0 = Neutral, 1 = Blue, 2 = Red, 3 = Green
};