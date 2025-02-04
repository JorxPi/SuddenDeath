#include "PassiveNine.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveNine::Activate(GameManager* gameManager, InvocationScript* invocation) {
    // Not used directly
}

void PassiveNine::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager || !invocation) return;

    // Increment combat wins for the invocation
    combatWins++;

    // Check if the invocation has won 2 combats
    if (combatWins == 2) {
        int playerOwner = invocation->getPlayerOwner();

        // Award 3 sudden points to the player
        gameManager->addPlayerPoints(playerOwner, 3);
        std::cout << "Player " << playerOwner << " gained 3 sudden points from PassiveNine!" << std::endl;

        combatWins = 0;
    }
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