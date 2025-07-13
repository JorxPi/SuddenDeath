#pragma once
#include "PassiveAbilities.h"
#include "GameManager.h"

class PassiveNine : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override; 
    const std::string& GetName() const override;

private:
    int combatWins = 0;
};
