#include "PassiveNine.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveNine::Activate(GameManager* gameManager, InvocationScript* invocation) {
    // Not used directly
}

void PassiveNine::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager || !invocation) return;

    int playerOwner = invocation->getPlayerOwner();

    // Award 3 sudden points to the player
    gameManager->addPlayerPoints(playerOwner, 2);
    std::cout << "Player " << playerOwner << " gained 2 sudden points from PassiveNine!" << std::endl;

    AnnouncePassiveUsage(gameManager, invocation);

    Reveal();
}

void PassiveNine::OnInvocationDeath() {
    // Reset the combat win counter when the invocation dies
    combatWins = 0;
    std::cout << "PassiveNine reset: combat win count cleared on invocation death." << std::endl;
}

const std::string& PassiveNine::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveNine; // Use the constant
    return description;
}