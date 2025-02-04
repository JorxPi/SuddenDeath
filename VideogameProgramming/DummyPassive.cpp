#include "DummyPassive.h"

// Override the Activate function to do nothing
void DummyPassive::Activate(GameManager* gameManager, InvocationScript* invocation) {
    // Intentionally left empty
}

// Override the GetDescription function to return the "No passive" description
const std::string& DummyPassive::GetDescription() const {
    static const std::string description = "No passive ability assigned.";
    return description;
}