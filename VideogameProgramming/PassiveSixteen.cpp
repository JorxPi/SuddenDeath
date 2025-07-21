#include "PassiveSixteen.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveSixteen::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager) return;

    int flipPlayer = gameManager->getCurrentPlayerTurn();
    int passiveOwner = invocation->getPlayerOwner();

    std::cout << "Player flipping tails : " << gameManager->getCurrentPlayerTurn() << ". Has " << gameManager->getPlayerScore(flipPlayer) << " points." << std::endl;
    std::cout << "Player holding passive : " << invocation->getPlayerOwner() << ". Has " << gameManager->getPlayerScore(passiveOwner) << " points." << std::endl;

    int flipPlayerScore = gameManager->getPlayerScore(flipPlayer) - 3;
    int passiveHolderScore = gameManager->getPlayerScore(passiveOwner);

    if (flipPlayerScore > passiveHolderScore) {
        gameManager->addPlayerPoints(flipPlayer, -3);
        gameManager->LogPlayerScore(flipPlayer, -3, "passive activation");


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