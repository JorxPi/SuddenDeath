#include "PassiveEleven.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"

void PassiveEleven::Activate(GameManager* gameManager, InvocationScript* invocation) {
    firstFlipHandled = false;
    std::cout << "PassiveEleven: Activated for invocation." << std::endl;
}

void PassiveEleven::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager) return;

    if (firstFlipHandled) return;

    firstFlipHandled = true; 

    std::cout << "PassiveEleven: First flip was tails! Respawning coin for a second flip." << std::endl;
}

void PassiveEleven::OnInvocationDeath() {
    firstFlipHandled = false;
    std::cout << "PassiveEleven: Invocation died. Resetting first flip tracking." << std::endl;
}

const std::string& PassiveEleven::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveEleven;
    return description;
}