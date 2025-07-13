#include "PassiveFive.h"
#include "InvocationScript.h"
#include "PassiveDescriptions.h"

void PassiveFive::Activate(GameManager* gameManager, InvocationScript* invocation) {
    PassiveAbility::Activate(gameManager, invocation);

    active = true;
    stationaryCounter.clear(); // Reset tracking

    // Track all enemy invocations on the board at activation time
    for (auto* enemyInvocation : gameManager->getInvocationScripts()) {
        if (enemyInvocation->getPlayerOwner() != invocation->getPlayerOwner()) {
            stationaryCounter[enemyInvocation] = 0;
        }
    }

    std::cout << "PassiveFive: Activation complete. Now tracking enemy invocations." << std::endl;
}

void PassiveFive::OnEvent(GameManager* gameManager, InvocationScript* invocation) {
    if (!active) return; // Don't do anything if PassiveFive isn't active
    std::cout << "PassiveFive: Event." << std::endl;

    // Synchronize `stationaryCounter` with the current board state
    synchronizeTracking(gameManager, invocation);

    int currentTurnPlayer = gameManager->getCurrentPlayerTurn();

    // Iterate over tracked invocations
    for (auto it = stationaryCounter.begin(); it != stationaryCounter.end();) {
        InvocationScript* enemyInvocation = it->first;

        // Check if this invocation belongs to the current player
        if (enemyInvocation->getPlayerOwner() != currentTurnPlayer) {
            ++it; // Skip invocations from other players
            continue;
        }

        if (enemyInvocation->getSkippedMovement()) {
            std::cout << "PassiveFive: Invocation at index "
                << gameManager->getInvocationIndex(enemyInvocation)
                << " skipped movement. Incrementing counter." << std::endl;

            // Increment skipped turn counter
            it->second++;

            int index = gameManager->getInvocationIndex(enemyInvocation);
            std::cout << "PassiveFive: Invocation index in list: " << index
                << ", Skipped turns in a row: " << it->second << "." << std::endl;

            if (it->second >= 2) {
                if (index != -1) {
                    // Destroy the invocation if it remained stationary for 2 turns
                    std::cout << "PassiveFive: Destroying invocation at index " << index << "." << std::endl;
                    gameManager->eliminateInvocation(index);

                    AnnouncePassiveUsage(gameManager);

                    Reveal();
                }

                // Remove from tracking
                it = stationaryCounter.erase(it);
                continue;
            }
        }
        else {
            // Reset counter if the invocation moved
            std::cout << "PassiveFive: Invocation at index "
                << gameManager->getInvocationIndex(enemyInvocation)
                << " moved. Resetting counter." << std::endl;
            it->second = 0;
        }
        ++it;
    }
}

void PassiveFive::synchronizeTracking(GameManager* gameManager, InvocationScript* invocation) {
    std::unordered_map<InvocationScript*, int> updatedCounter;

    // Iterate over all current enemy invocations on the board
    for (auto* enemyInvocation : gameManager->getInvocationScripts()) {
        // Track only enemy invocations
        if (enemyInvocation->getPlayerOwner() != invocation->getPlayerOwner()) {
            if (stationaryCounter.find(enemyInvocation) != stationaryCounter.end()) {
                // Preserve the skipped turn counter if already tracked
                updatedCounter[enemyInvocation] = stationaryCounter[enemyInvocation];
            }
            else {
                // Initialize skipped turn counter for new enemy invocations
                updatedCounter[enemyInvocation] = 0;
            }
        }
    }

    // Replace the old counter with the updated one
    stationaryCounter = std::move(updatedCounter);

    std::cout << "PassiveFive: Tracking synchronized. Now tracking " << stationaryCounter.size() << " enemy invocations." << std::endl;
}

void PassiveFive::OnInvocationDeath() {
    // Clear all tracking when the invocation dies
    stationaryCounter.clear();
    std::cout << "PassiveFive: State reset on invocation death." << std::endl;
}

const std::string& PassiveFive::GetDescription() const {
    static const std::string description = PassiveDescriptions::GetDescription(4);
    return description;
}

const std::string& PassiveFive::GetName() const {
    static const std::string name = PassiveDescriptions::GetName(4);
    return name;
}