#include "PassiveSeven.h"
#include "GameManager.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveSeven::OnInvocationDeath() {
    std::cout << "PassiveSeven: Invocation with this passive has died." << std::endl;
}

const std::string& PassiveSeven::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(6);
    return description;
}

const std::string& PassiveSeven::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(6);
    return name;
}

void PassiveSeven::OnEvent(GameManager* gameManager, InvocationScript* opponentInvocation) {
    if (!gameManager || !opponentInvocation) return;

    // Reduce opponent's attack by 1 (permanently)
    int newAttack = opponentInvocation->getAttack() - 1;
    if (newAttack < 0) newAttack = 0;  // Ensure attack doesn't go below 0
    opponentInvocation->setInvocAttack(newAttack);

    std::cout << "PassiveSeven: Reduced opponent's attack by 1 before battle. New attack: " << newAttack << std::endl;

    // **Trigger the announcement**
    AnnouncePassiveUsage(gameManager);

    Reveal();
}