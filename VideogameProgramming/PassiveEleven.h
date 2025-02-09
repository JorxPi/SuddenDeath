#pragma once
#include "PassiveAbilities.h"
#include "GameManager.h"

class PassiveEleven : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override;
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;
    void ResetTurnState();
    int HowManyFlipsHandled();

private:
    int flipsHandled = 0; 
};
