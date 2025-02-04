#pragma once
#include "PassiveAbilities.h"
#include <string>

// DummyPassive class: A placeholder passive ability that does nothing
class DummyPassive : public PassiveAbility {
public:
    void Activate(GameManager* gameManager, InvocationScript* invocation) override; // Override Activate
    const std::string& GetDescription() const override; // Override GetDescription
};
