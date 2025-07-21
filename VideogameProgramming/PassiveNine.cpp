#include "PassiveNine.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveNine::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager || !invocation) return;

    int playerOwner = invocation->getPlayerOwner();

    // Award 2 sudden points to the player
    gameManager->addPlayerPoints(playerOwner, 2);

    gameManager->LogPlayerScore(playerOwner, 2, "passive activation");

    std::cout << "Player " << playerOwner << " gained 2 sudden points from PassiveNine!" << std::endl;

    AnnouncePassiveUsage(gameManager);

    Reveal();
}

void PassiveNine::OnInvocationDeath() {
    // Reset the combat win counter when the invocation dies
    combatWins = 0;
    std::cout << "PassiveNine reset: combat win count cleared on invocation death." << std::endl;
}

const std::string& PassiveNine::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(8); // Use the constant
    return description;
}

const std::string& PassiveNine::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(8);
    return name;
}