#include "PassiveThree.h"
#include "CellScript.h"
#include "GameManager.h"  
#include "PassiveDescriptions.h"

void PassiveThree::Activate(GameManager* gameManager, InvocationScript* invocation) {
    PassiveAbility::Activate(gameManager, invocation);
    hasActivated = false;
}

void PassiveThree::OnEvent(GameManager* gm, InvocationScript* inv) {
    if (!gm || hasActivated) return;
    if (gm->getCurrentGameTurn() < 5) return;

    int playerId = inv->getPlayerOwner();

    bool ownsAny = false;
    for (CellScript* cell : gm->getAllCellScripts()) {
        if (cell->getIsConquerable() && cell->getOwner() == playerId) {
            ownsAny = true;
            break;
        }
    }

    if (!ownsAny) {
        gm->addPlayerPoints(playerId, +6);
        hasActivated = true;

        AnnouncePassiveUsage(gm);
        Reveal();
    }
}

const std::string& PassiveThree::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(2);
    return description;
}

const std::string& PassiveThree::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(2);
    return name;
}