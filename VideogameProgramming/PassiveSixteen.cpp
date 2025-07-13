#include "PassiveSixteen.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveSixteen::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager) return;

    int flipPlayer = gameManager->getCurrentPlayerTurn();
    int passiveOwner = invocation->getPlayerOwner();

    int flipPlayerScore = gameManager->getPlayerScore(flipPlayer);
    int passiveHolderScore = gameManager->getPlayerScore(passiveOwner);

    if (flipPlayerScore > passiveHolderScore) {
        gameManager->addPlayerPoints(flipPlayer, -3);

        AnnouncePassiveUsage(gameManager);
        Reveal();
    }
}

const std::string& PassiveSixteen::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(15);
    return description;
}

const std::string& PassiveSixteen::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(15);
    return name;
}