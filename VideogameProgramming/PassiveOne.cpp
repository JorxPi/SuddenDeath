#include "PassiveOne.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveOne::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (invocation->getAttack() == 6) return;

    int newAttack = invocation->getAttack() + 1;

    invocation->setInvocAttack(newAttack); 

    AnnouncePassiveUsage(gameManager);

    Reveal();   
}

void PassiveOne::OnInvocationDeath() {
    std::cout << "PassiveOne: Invocation with this passive has died." << std::endl;
}

const std::string& PassiveOne::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(0);
    return description;
}

const std::string& PassiveOne::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(0);
    return name;
}