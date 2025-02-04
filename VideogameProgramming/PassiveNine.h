#pragma once
#include "PassiveAbilities.h"
#include "GameManager.h"

class PassiveNine : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override;
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override; 

private:
    int combatWins = 0;
};
