#include "PassiveOne.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveOne::Activate(GameManager* gameManager, InvocationScript* invocation) {
    // Not used directly
}

void PassiveOne::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    skippedTurns++;

    if (skippedTurns == 1 && !stationaryPassiveActivated) {
        int newAttack = invocation->getAttack() + 2;

        if (newAttack > 6) {
            newAttack = 6;
        }

        invocation->setInvocAttack(newAttack); 
        stationaryPassiveActivated = true;

        const char* attackTexture = gameManager->returnAttackText(newAttack);
        invocation->updateAttackSprite(attackTexture);

        AnnouncePassiveUsage(gameManager, invocation);

        Reveal();

        std::cout << "PassiveOne activated: Attack increased to " << newAttack << std::endl;
    }
}

void PassiveOne::OnInvocationDeath() {
    skippedTurns = 0;
    std::cout << "PassiveOne reset: skipped turns count cleared on invocation death." << std::endl;
}

const std::string& PassiveOne::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveOne;
    return description;
}