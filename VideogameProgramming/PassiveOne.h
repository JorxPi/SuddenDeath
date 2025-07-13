#pragma once
#include "PassiveAbilities.h"
#include "GameManager.h"

class PassiveOne : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    void OnInvocationDeath() override;
    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

private:                   
    bool stationaryPassiveActivated = false; 
};
