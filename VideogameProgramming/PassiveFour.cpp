#include "PassiveFour.h"
#include "InvocationScript.h"
#include "GameManager.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveFour::OnEvent(GameManager* gameManager, InvocationScript* invocation) {

}

void PassiveFour::OnInvocationDeath() {
    std::cout << "PassiveFour: Invocation with this passive has died." << std::endl;
}

const std::string& PassiveFour::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(3);
    return description;
}

const std::string& PassiveFour::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(3);
    return name;
}

void PassiveFour::CheckAndSwapWinner(GameManager* gameManager, InvocationScript* invocation, int& winningIndex, int& losingIndex) {
    if (!gameManager || !invocation) return;

    std::swap(winningIndex, losingIndex);

    AnnouncePassiveUsage(gameManager);

    Reveal();
}