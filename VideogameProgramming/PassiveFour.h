#pragma once
#include "PassiveAbilities.h"

class PassiveFour : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

    void CheckAndSwapWinner(GameManager* gameManager, InvocationScript* invocation, int& winningIndex, int& losingIndex);
};
