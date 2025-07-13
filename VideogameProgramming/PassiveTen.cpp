#include "PassiveTen.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"

void PassiveTen::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager) return;

    for (int i = 1; i <= 3; i++) {
        if (i != invocation->getPlayerOwner()) { 
            gameManager->addPlayerPoints(i, -1);
        }
    }

    AnnouncePassiveUsage(gameManager);

    Reveal();  
}

void PassiveTen::OnInvocationDeath() {
    combatWins = 0; // Reset the counter when the invocation dies
    std::cout << "PassiveTen: Invocation died. Resetting combat win count." << std::endl;
}

const std::string& PassiveTen::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(9);
    return description;
}

const std::string& PassiveTen::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(9);
    return name;
}