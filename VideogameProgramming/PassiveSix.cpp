#include "PassiveSix.h"
#include "GameManager.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveSix::Activate(GameManager* gameManager, InvocationScript* invocation) {
    ownerInvocation = invocation;
}

void PassiveSix::OnEvent(GameManager* gameManager, InvocationScript* summonedInvocation) {
    if (!gameManager || !summonedInvocation) return;

    int summonedAttack = summonedInvocation->getAttack();
    int passiveSixAttack = ownerInvocation->getAttack(); 

    if (summonedAttack > passiveSixAttack) {
        int newAttack = summonedAttack - 1;
        summonedInvocation->setInvocAttack(newAttack);

        // Update attack sprite
        //const char* attackTexture = gameManager->returnAttackText(newAttack);
        //summonedInvocation->updateAttackSprite(attackTexture);

        std::cout << "PassiveSix: Summoned invocation had more attack, reducing attack by 1. New attack: " << newAttack << std::endl;
        Reveal();
    }
}

void PassiveSix::OnInvocationDeath() {
    std::cout << "PassiveSix: Invocation with this passive has died." << std::endl;
}

const std::string& PassiveSix::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveSix;
    return description;
}