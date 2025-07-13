#pragma once
#include "PassiveAbilities.h"

class PassiveSeven : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

private:
    int originalAttack = -1;
};