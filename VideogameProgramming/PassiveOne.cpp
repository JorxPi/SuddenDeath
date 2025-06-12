#include "PassiveOne.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveOne::Activate(GameManager* gameManager, InvocationScript* invocation) {
    // Not used directly
}

void PassiveOne::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (invocation->getAttack() == 6) return;

    int newAttack = invocation->getAttack() + 1;

    if (newAttack > 6) {
        newAttack = 6;
    }

    invocation->setInvocAttack(newAttack); 

    const char* attackTexture = gameManager->returnAttackText(newAttack);
    invocation->updateAttackSprite(attackTexture);

    AnnouncePassiveUsage(gameManager, invocation);

    Reveal();

    std::cout << "PassiveOne activated: Attack increased to " << newAttack << std::endl;
   
}

void PassiveOne::OnInvocationDeath() {
    skippedTurns = 0;
    std::cout << "PassiveOne reset: skipped turns count cleared on invocation death." << std::endl;
}

const std::string& PassiveOne::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveOne;
    return description;
}