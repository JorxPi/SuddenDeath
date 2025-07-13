#pragma once
#include "PassiveAbilities.h"

class PassiveSix : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* summonedInvocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

private:
    InvocationScript* ownerInvocation = nullptr;
};