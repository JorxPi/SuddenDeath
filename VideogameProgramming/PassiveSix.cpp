#include "PassiveSix.h"
#include "GameManager.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>


void PassiveSix::OnEvent(GameManager* gameManager, InvocationScript* summonedInvocation) {
    if (!gameManager || !summonedInvocation) return;

    int summonedAttack = summonedInvocation->getAttack();
    int passiveSixAttack = invocationRef->getAttack(); 

    if (summonedAttack > passiveSixAttack) {
        int newAttack = summonedAttack - 1;
        summonedInvocation->setInvocAttack(newAttack);

        AnnouncePassiveUsage(gameManager);

        Reveal();
    }
}

void PassiveSix::OnInvocationDeath() {
    std::cout << "PassiveSix: Invocation with this passive has died." << std::endl;
}

const std::string& PassiveSix::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(5);
    return description;
}

const std::string& PassiveSix::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(5);
    return name;
}