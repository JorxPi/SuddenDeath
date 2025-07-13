#include "PassiveEleven.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"

void PassiveEleven::Activate(GameManager* gameManager, InvocationScript* invocation) {
    PassiveAbility::Activate(gameManager, invocation);
    flipsHandled = 0;
}

void PassiveEleven::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager) return;

    flipsHandled++;

    AnnouncePassiveUsage(gameManager);

    Reveal();

    std::cout << "PassiveEleven: First flip was tails! Respawning coin for a second flip." << std::endl;
}

void PassiveEleven::OnInvocationDeath() {
    flipsHandled = 0;
    std::cout << "PassiveEleven: Invocation died. Resetting first flip tracking." << std::endl;
}

const std::string& PassiveEleven::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(10);
    return description;
}

const std::string& PassiveEleven::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(10);
    return name;
}

void PassiveEleven::ResetTurnState() {
    flipsHandled = 0;
}

int PassiveEleven::HowManyFlipsHandled() {
    return flipsHandled;
}
