#include "PassiveAbilities.h"
#include "GameManager.h"
#include "InvocationScript.h"

// Announce passive ability usage
void PassiveAbility::AnnouncePassiveUsage(GameManager* gameManager, InvocationScript* invocation) {
    if (!gameManager || !invocation) return;

    int playerOwner = invocation->getPlayerOwner();
    std::string playerColor = gameManager->getPlayerColor(playerOwner);
    std::string passiveDescription = GetDescription();

    std::string announcement = playerColor + " player invocation passive used: " + passiveDescription;
    gameManager->ShowAnnouncement(announcement, 12.0f, AnnouncementType::PassiveTrigger);
    gameManager->LogPassiveUsage(invocation);
}

// Get passive ability description
const std::string& PassiveAbility::GetDescription() const {
    static const std::string defaultDescription = "No description available.";
    return defaultDescription;
}