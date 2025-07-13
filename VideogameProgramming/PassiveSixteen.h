#pragma once
#include "PassiveAbilities.h"
#include "GameManager.h"

class PassiveSixteen : public PassiveAbility {
public:
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;
    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

private:

};