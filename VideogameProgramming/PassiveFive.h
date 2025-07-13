#pragma once
#include "PassiveAbilities.h"
#include "GameManager.h"
#include <unordered_map> // To track skipped turns

class PassiveFive : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override; 
    void OnInvocationDeath() override; 
    void Activate(GameManager* gameManager, InvocationScript* invocation) override; 
    void synchronizeTracking(GameManager* gameManager, InvocationScript* ownerInvocation);

    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

private:
    std::unordered_map<InvocationScript*, int> stationaryCounter; // Track skipped turns for relevant invocations
    bool active = false; // Whether PassiveFive has been activated
};
