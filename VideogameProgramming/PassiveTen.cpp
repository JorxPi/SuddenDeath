#include "PassiveTen.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"

void PassiveTen::Activate(GameManager* gameManager, InvocationScript* invocation) {
    //No activation
}

void PassiveTen::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager) return;

    std::cout << "PassiveTen: Invocation won a combat, All rival players lose 1 sudden point." << std::endl;

    // Deduct 1 points from all rival players
    for (int i = 1; i <= 3; i++) {
        if (i != invocation->getPlayerOwner()) { 
            gameManager->addPlayerPoints(i, -1);
        }
    }

    AnnouncePassiveUsage(gameManager, invocation);

    Reveal();
    
}

void PassiveTen::OnInvocationDeath() {
    combatWins = 0; // Reset the counter when the invocation dies
    std::cout << "PassiveTen: Invocation died. Resetting combat win count." << std::endl;
}

const std::string& PassiveTen::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveTen;
    return description;
}