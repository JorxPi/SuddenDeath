#pragma once
#include "Script.h"
#include "PassiveAbilities.h"
#include <vector>

class CellScript;

class PassiveFifteen : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override;
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void ModifyMovableCells(GameManager* gameManager, InvocationScript* invocation, std::vector<CellScript*>& movableCellScripts, std::vector<Entity*>& movableCells);

    const std::string& GetDescription() const override;
    const std::string& GetName() const override;
    int GetTurnsSurvived() const { return turnsSurvived; }

private:
    int turnsSurvived = 0;
};