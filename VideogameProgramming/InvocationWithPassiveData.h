#pragma once

#include <string>
#include <memory>
#include "PassiveAbilities.h"

// Struct to store invocation sprite filepath and associated passive
struct InvocationWithPassiveData {
    std::string filepath;               // Path to the sprite
    std::unique_ptr<PassiveAbility> passive; // Passive ability
};
