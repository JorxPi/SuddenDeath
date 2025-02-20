#pragma once
#include "PassiveAbilities.h"

class PassiveSeven : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override;
    void OnEvent(GameManager* gameManager, InvocationScript* opponentInvocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;

private:
    int originalAttack = -1;
};