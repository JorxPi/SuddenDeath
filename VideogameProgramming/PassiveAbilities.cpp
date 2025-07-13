#include "PassiveAbilities.h"
#include "GameManager.h"
#include "InvocationScript.h"
#include "InvocationConstants.h"

void PassiveAbility::Activate(GameManager* gameManager, InvocationScript* invocation) {
    invocationRef = invocation;
}

// Announce passive ability usage
void PassiveAbility::AnnouncePassiveUsage(GameManager* gameManager) {
    if (!gameManager || !invocationRef) return;

    const std::string& path = invocationRef->getInvocationEntity()->get<Sprite>()->filepath;

    // 2) scan your Invocations::data[] for that same path
    const char* invName = "Unknown";
    for (int i = 0; i < Invocations::numInvocations; ++i) {
        if (path == Invocations::data[i].texturePath) {
            invName = Invocations::data[i].name;
            break;
        }
    }

    std::string passiveDescription = GetDescription();
    std::string passiveName = GetName();

    int playerOwner = invocationRef->getPlayerOwner();
    std::string playerColor = gameManager->getPlayerColor(playerOwner);

    std::string announcement = playerColor + " player " + invName + " used " + passiveName + " passive";
    gameManager->ShowAnnouncement(announcement, 12.0f, AnnouncementType::PassiveTrigger);
}

// Get passive ability description
const std::string& PassiveAbility::GetDescription() const {
    static const std::string defaultDescription = "No description available.";
    return defaultDescription;
}

const std::string& PassiveAbility::GetName() const {
    static const std::string defaultName = "No name available.";
    return defaultName;
}