#pragma once
#include "PassiveAbilities.h"

class PassiveTwo : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

    void TriggerPassiveEffect(GameManager* gameManager, InvocationScript* passiveHolder, InvocationScript* enemyInvocation);
};
