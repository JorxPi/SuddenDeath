#include "PassiveTwo.h"
#include "GameManager.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveTwo::OnEvent(GameManager* gameManager, InvocationScript* invocation) {

}

const std::string& PassiveTwo::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveTwo;
    return description;
}

void PassiveTwo::TriggerPassiveEffect(GameManager* gameManager, InvocationScript* passiveHolder, InvocationScript* enemyInvocation) {
    if (!gameManager || !passiveHolder || !enemyInvocation) return;

    // Reduce attack by 2, ensuring it doesn’t go below 1
    int newAttack = enemyInvocation->getAttack() - 2;
    if (newAttack < 1) newAttack = 1;
    enemyInvocation->setInvocAttack(newAttack);

    // **Trigger the announcement**
    AnnouncePassiveUsage(gameManager, passiveHolder);

    Reveal();
}