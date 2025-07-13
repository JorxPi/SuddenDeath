#include "PassiveTwo.h"
#include "GameManager.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

const std::string& PassiveTwo::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(1);
    return description;
}

const std::string& PassiveTwo::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(1);
    return name;
}

void PassiveTwo::OnEvent(GameManager* gameManager, InvocationScript* enemyInvocation) {
    if (!gameManager || !enemyInvocation) return;

    // Reduce attack by 2, ensuring it doesn’t go below 1
    int newAttack = enemyInvocation->getAttack() - 2;
    if (newAttack < 1) newAttack = 1;
    enemyInvocation->setInvocAttack(newAttack);

    // **Trigger the announcement**
    AnnouncePassiveUsage(gameManager);

    Reveal();
}