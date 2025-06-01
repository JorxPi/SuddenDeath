#pragma once

#include "Script.h"
#include "CellScript.h"
#include "GameManager.h"
#include "ButtonManager.h"
#include "PassiveAbilities.h"
#include "PassiveFifteen.h"

class GameManager;

class ButtonManager;

class PassiveAbility;

class InvocationScript : public Script
{
    using Script::Script;

public:
    //GENERAL LOGIC
    void startScript() override;
    void tickScript(float deltaTime) override;
    void moveInvocation(int posX, int posY);
    void highlightCells();
    void clearHighlightedCells();
    //void destroyInvocation();
    bool checkInvocInSpawnCells();
    void updateAttackSprite(const char* newAttackTexture);
    void undoLastMove();

    void incrementSkippedTurns();   
    void resetSkippedTurns();

    //GETTERS
    bool getFullyMoved();
    int getAttack();
    bool getBattleChecked();
    Entity* getInvocationEntity();
    ECS::Entity* getCardBackEntity();
    ECS::Entity* getAttackNumberEntity();
    bool getMoveAllowed();
    bool getMoved();
    bool getRightClickPressed();
    int getPlayerOwner();

    int getSkippedTurns();
    bool getSkippedMovement();

    PassiveAbility* getPassiveAbility();

    //SETTERS
    void setGameManagerRef(GameManager* gmr);
    void setButtonManagerRef(ButtonManager* bmr);
    void setInvocRefs(Entity* invr, Entity* cbr, Entity* attr);
    void setMovedAndCheckBattleToFalse();
    void setFullyMovedToTrue();
    void setMoveAllowedToTrue();
    void setInvocAttack(int invAttack);
    void setBattleCheckedToTrue();
    void setMoved(bool value);
    void setRightClickPressed(bool value);
    void setPlayerOwner(int owner);
    void setSkippedMovement(bool value);
    void setPreviousPos(int posX, int posY);
    void setRecentlySummoned(bool value);
    void setHasBattled(bool value);

    void setPassiveAbility(PassiveAbility* passive);


private:
    GameManager* gameRef;
    ButtonManager* buttonRef;
    int attack = 0;
    int playerOwner;
    bool moveAllowed = false;
    bool moved = false;
    bool battleChecked = false;
    bool fullyMoved = false;
    bool buttonCreated = false;
    bool rightClickPressed = false;
    int skippedTurns = 0;
    bool skippedMovement = false;
    bool canUndo = false;
    bool recentlySummoned = true;
    bool hasBattled = false;

    //real pos to move
    glm::vec2 lPos;
    //pos while moving to lPos
    glm::vec2 tPos;

    std::vector<Entity*> movableCells;
    std::vector<CellScript*> movableCellScripts;

    glm::vec2 previousPos;

    Entity* invocationEnt;
    Entity* cardBackEnt;
    Entity* attackNumberEnt;

    PassiveAbility* passiveAbility;

};
