#pragma once
#include "PassiveAbilities.h"

class PassiveTwo : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override {}
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    const std::string& GetDescription() const override;

    void TriggerPassiveEffect(GameManager* gameManager, InvocationScript* passiveHolder, InvocationScript* enemyInvocation);
};
