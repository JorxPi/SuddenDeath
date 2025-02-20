#pragma once
#include "PassiveAbilities.h"

class PassiveSix : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override;
    void OnEvent(GameManager* gameManager, InvocationScript* summonedInvocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;

private:
    InvocationScript* ownerInvocation = nullptr;
};