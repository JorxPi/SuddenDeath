#pragma once
#include "Script.h"
#include "PassiveAbilities.h"
#include <vector>

class PassiveThree : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override;
    void OnEvent(GameManager* gameManager, InvocationScript* invocation) override;

    const std::string& GetDescription() const override;
    const std::string& GetName() const override;

private:
    bool hasActivated;
};
