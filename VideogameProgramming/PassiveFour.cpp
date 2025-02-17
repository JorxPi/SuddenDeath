#include "PassiveFour.h"
#include "InvocationScript.h"
#include "GameManager.h"
#include "PassiveDescriptions.h"
#include <iostream>

void PassiveFour::Activate(GameManager* gameManager, InvocationScript* invocation) {
}

void PassiveFour::OnEvent(GameManager* gameManager, InvocationScript* invocation) {

}

void PassiveFour::OnInvocationDeath() {
    std::cout << "PassiveFour: Invocation with this passive has died." << std::endl;
}

const std::string& PassiveFour::GetDescription() const {
    static const std::string description = PassiveDescriptions::PassiveFour;
    return description;
}

void PassiveFour::CheckAndSwapWinner(GameManager* gameManager, int& winningIndex, int& losingIndex) {
    if (!gameManager) return;

    std::swap(winningIndex, losingIndex);

    Reveal();
}