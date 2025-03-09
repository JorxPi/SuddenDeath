#pragma once
#include "PassiveAbilities.h"

class PassiveFour : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override;
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;

    void CheckAndSwapWinner(GameManager* gameManager, InvocationScript* invocation, int& winningIndex, int& losingIndex);
};
