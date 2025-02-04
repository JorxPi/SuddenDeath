#include "GameManager.h"
#include "CellConstants.h"
#include <list>



GameManager::GameManager(GLFWwindow* window, ECS::World* world, EntityManager* entityManager)
    : window(window), world(world), entityManager(entityManager) {
}

ECS::Entity* GameManager::createEntity(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, bool autoSize, glm::vec2 size, const char* shaderName) {
    ECS::Entity* entity = world->create();
    entity->assign<Transform>(position, rotation, scale);
    entity->assign<Sprite>(filepath, color, autoSize, size, shaderName);

    return entity;
}

void GameManager::activate(bool status) {
    isActive = status;
}

void GameManager::update(float deltaTime) {
    if (!isActive) return;

    if (!currentPlayerInvocationsListSetted) {
        setCurrentPlayerInvocationsListRef();
        currentPlayerInvocationsListSetted = true;
    }

    if (!ruletaBatallaSpawned) {
        //checkingIfBattle = true;
        for (int i = 0; i < currentPlayerInvocationsListRef.size(); i++) {
            if (currentPlayerInvocationsListRef[i]->getFullyMoved() && !currentPlayerInvocationsListRef[i]->getBattleChecked()) {
                currentPlayerInvocationsListRef[i]->setBattleCheckedToTrue();
                std::cout << "1. " << std::endl;
                checkingIfBattle = true;
                checkBattle(currentPlayerInvocationsListRef[i]);
            }
        }
    }

    if (deferredCoinSpawn && !battleing && !checkingIfBattle) {
        movementBlocked = true;
        spawnCoin();
        deferredCoinSpawn = false;
    }

    if (ruletaBatallaDestroyed && !invocDestroyed) {
        if (ruletaBatallaWinner == 1) {
            invocationsListRef[posListInvoc1]->moveInvocation(invocationsListRef[posListInvoc1]->getInvocationEntity()->get<Transform>()->position.x + 40, invocationsListRef[posListInvoc1]->getInvocationEntity()->get<Transform>()->position.y);
            awardBattlePoints(posListInvoc1, posListInvoc2);

            handlePassiveEvent<PassiveNine>(invocationsListRef[posListInvoc1], [this, winner = invocationsListRef[posListInvoc1]](PassiveNine* passive) {
                passive->OnEvent(this, winner);
                });

            handlePassiveEvent<PassiveTen>(invocationsListRef[posListInvoc1], [this](PassiveTen* passive) {
                passive->OnEvent(this, invocationsListRef[posListInvoc1]);
                });
            
            eliminateInvocation(posListInvoc2);
        }
        else {
            invocationsListRef[posListInvoc2]->moveInvocation(invocationsListRef[posListInvoc2]->getInvocationEntity()->get<Transform>()->position.x - 40, invocationsListRef[posListInvoc2]->getInvocationEntity()->get<Transform>()->position.y);
            awardBattlePoints(posListInvoc2, posListInvoc1);

            handlePassiveEvent<PassiveNine>(invocationsListRef[posListInvoc2], [this, winner = invocationsListRef[posListInvoc2]](PassiveNine* passive) {
                passive->OnEvent(this, winner);
                });

            handlePassiveEvent<PassiveTen>(invocationsListRef[posListInvoc1], [this](PassiveTen* passive) {
                passive->OnEvent(this, invocationsListRef[posListInvoc1]);
                });
            
            eliminateInvocation(posListInvoc1);
        }

        battleing = false;
        ruletaBatallaDestroyed = false;
        invocDestroyed = true;
        ruletaBatallaSpawned = false;
    }

    if (!battleing) {
        if (!movementBlocked) {
            int movedInvocs = 0;
            for (int j = 0; j < currentPlayerInvocationsListRef.size(); j++) {
                InvocationScript* currentPlayerInvocationScript = currentPlayerInvocationsListRef[j];

                if (currentPlayerInvocationScript) {
                    bool moved = currentPlayerInvocationScript->getFullyMoved();

                    if (moved) {
                        movedInvocs++;
                    }
                }
            }

            if (movedInvocs == currentPlayerInvocationsListRef.size()) {
                allInvocsMoved = true;
                deferredCoinSpawn = true;
            }
        }
    }
    
    if (coinScriptRef && invocDestroyed) {
        if (coinScriptRef->getCoinFlipped()) {
            auto flipTime = coinScriptRef->getFlipTime();
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - flipTime);

            if (elapsed.count() >= 3) {
                bool headOrTails = coinScriptRef->getHeadOrTails();
                world->destroy(coinEntRef);
                coinEntRef = nullptr;
                coinScriptRef = nullptr;

                bool hasPassiveEleven = false;

                for (auto* invocation : currentPlayerInvocationsListRef) {
                    if (auto* passive = invocation->getPassiveAbility()) {
                        if (dynamic_cast<PassiveEleven*>(passive)) {
                            hasPassiveEleven = true;
                            break;
                        }
                    }
                }

                if (hasPassiveEleven && !headOrTails) { // First flip is tails, PassiveEleven active
                    std::cout << "PassiveEleven active: First tails flip detected, no points awarded. Respawning coin." << std::endl;

                    // Trigger PassiveEleven
                    handlePassiveEvent<PassiveEleven>(currentPlayerInvocationsListRef[0], [this](PassiveEleven* passive) {
                        passive->OnEvent(this, currentPlayerInvocationsListRef[0]);
                        });

                    spawnCoin(); // Spawn the second coin
                    return; // Stop execution to wait for second coin flip
                }

                // Normal coin flip resolution
                if (headOrTails) {
                    startInvocationSelection();
                    selectingInvocation = true;
                }
                else {
                    playerScores[currentPlayerTurn] += 3;
                    std::cout << "Tails coin points awarded to Player " << currentPlayerTurn
                        << " now has " << playerScores[currentPlayerTurn] << " points." << std::endl;
                    updateScoreVisibility(currentPlayerTurn);
                    changeTurnAndReset();
                }
            }
        }
    }

    if (ruletaAttacksDestroyed) {
        changeTurnAndReset();
    } 

    if (ruletaAttacksSpawned && ruletaAttacksRef->getInvocated() && !invocInvocated) {
        /*if (invocationToSpawnPos != -1) {
            invocInvocation(ruletaAttacksRef->getSpinResult(), invocCardListRef[invocationToSpawnPos]->getSpriteFilepath());
            invocInvocated = true;
            invocationToSpawnPos = -1;
        }*/

        if (invocationToSpawnPos != -1) {
            InvocationSelectionScript* selectedScript = invocCardListRef[invocationToSpawnPos];

            const char* filepath = selectedScript->getSpriteFilepath(); // Get filepath
            PassiveAbility* passive = selectedScript->getPassiveAbility(); // Get passive

            invocInvocation(ruletaAttacksRef->getSpinResult(), filepath, passive); // Pass both

            invocInvocated = true;
            invocationToSpawnPos = -1;
        }
    }

    //Selection invocation

    if (selectingInvocation) {
        for (int i = 0; i < invocCardListRef.size(); i++) {
            if (invocCardListRef[i]->getCardSelected()) {
                selectingInvocation = false;
                invocationToSpawnPos = i;

                selectionTime = std::chrono::steady_clock::now();
                delayStarted = true;
                break;
            }
        }
    }

    if (delayStarted) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - selectionTime);

        if (elapsed.count() >= 3) {
            invocCardListRef[invocationToSpawnPos]->setCardSelected(false);
            endInvocationSelection();

            spawnRuletaAttacks();
            ruletaAttacksSpawned = true;


            delayStarted = false;
        }
    }

    if (ruletaAttacksSpawned && ruletaAttacksRef->getReadyToDestroy() && ruletaAttacksEntRef) {
        destroyRuletaAttacks();
        invocInvocated = false;
    }

    if (ruletaBatallaSpawned && ruletaBatallaRef->getReadyToDestroy() && ruletaBatallaEntRef) {
        destroyRuletaBatalla();
    }

    skipButtonGameLogic();  

    //renderSystem->RenderText("Player 1 Turn", 200.0f, 400.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

}



//GENERAL LOGIC



void GameManager::invocInvocation(int invocAttack, const char* filepath, PassiveAbility* passive) {
    switch (currentPlayerTurn) {
    /*case 1:
    {
        //entityManager->createInvocation(glm::vec2(670.f, 110.f), 1, invocAttack, "Textures/blue_back.png", "Textures/inv_Gobobo.png", returnAttackText(invocAttack));
        entityManager->createInvocation(glm::vec2(670.f, 110.f), 1, invocAttack, "Textures/blue_back.png", filepath, returnAttackText(invocAttack), currentPlayerTurn);
    }
    break;
    case 2:
    {
        entityManager->createInvocation(glm::vec2(950.f, 600.f), 2, invocAttack, "Textures/red_back.png", filepath, returnAttackText(invocAttack), currentPlayerTurn);
    }
    break;
    case 3:
    {
        entityManager->createInvocation(glm::vec2(300.f, 570.f), 3, invocAttack, "Textures/green_back.png", filepath, returnAttackText(invocAttack), currentPlayerTurn);
    }
    break;*/
    case 1:
        entityManager->createInvocationWithPassive(
            glm::vec2(670.f, 110.f), 1, invocAttack, "Textures/blue_back.png", filepath, returnAttackText(invocAttack), currentPlayerTurn, passive);
        break;
    case 2:
        entityManager->createInvocationWithPassive(
            glm::vec2(950.f, 600.f), 2, invocAttack, "Textures/red_back.png", filepath, returnAttackText(invocAttack), currentPlayerTurn, passive);
        break;
    case 3:
        entityManager->createInvocationWithPassive(
            glm::vec2(300.f, 570.f), 3, invocAttack, "Textures/green_back.png", filepath, returnAttackText(invocAttack), currentPlayerTurn, passive);
        break;
    default:
        break;
    }

    addInvocationToBoard(filepath);

    InvocationScript* invocScript = invocationsListRef.back(); // Assuming the last created invocation is added to the list
    awardInvocationPoints(invocScript);

    if (auto* passiveFive = dynamic_cast<PassiveFive*>(passive)) {
        passiveFive->Activate(this, invocScript); // Activate PassiveFive
    }
}

void GameManager::highlightSprite(int cellPos) {
    CellScript* cellScript = cellListRef[cellPos];

    Entity* cell = cellScript->getCellEntity();
    glm::vec2 pos = cell->get<Transform>()->position;

    Entity* highlight_cell;

    if (pos == CellPos::ha || pos == CellPos::hr || pos == CellPos::hg) {
        highlight_cell = entityManager->createEntityWithoutScript(pos, 0.f, 1.f, "Textures/SpawnHighlight.png", glm::vec3(1., 1., 1.));
    }
    else if (pos == CellPos::ta || pos == CellPos::tr || pos == CellPos::tg) {
        highlight_cell = entityManager->createEntityWithoutScript(pos, 0.f, 1.f, "Textures/GoodHighlight.png", glm::vec3(1., 1., 1.));
    }
    else {
        highlight_cell = entityManager->createEntityWithoutScript(pos, 0.f, 1.f, "Textures/resalto_ovalo.png", glm::vec3(1., 1., 1.));
    }

    highlightedEntities.push_back(highlight_cell);
}

void GameManager::changeTurnAndReset() {

    for (auto* invocation : invocationsListRef) {
        if (auto* passive = invocation->getPassiveAbility()) {
            // Check if the passive is PassiveFive
            if (auto* passiveFive = dynamic_cast<PassiveFive*>(passive)) {
                passiveFive->OnEvent(this, invocation);
            }
        }
    }

    // Reset skipped movement for the current player’s invocations
    for (auto* invocation : currentPlayerInvocationsListRef) {
        invocation->setSkippedMovement(false);
    }

    ruletaAttacksDestroyed = false;
    if (currentPlayerTurn != 3) {
        currentPlayerTurn = currentPlayerTurn++;
    }
    else {
        currentPlayerTurn = 1;
    }
    movementBlocked = false;
    currentPlayerInvocationsListRef.clear();
    currentPlayerInvocationsListSetted = false;
}

bool GameManager::checkIfAllyInvocInCell(int cellPosInArray) {
    Entity* cell = cellListRef[cellPosInArray]->getCellEntity();
    float cellPos = cell->get<Transform>()->position.x;

    for (int i = 0; i < invocationsListRef.size(); i++) {
        Entity* invocEntity = invocationsListRef[i]->getInvocationEntity();
        if (invocEntity->get<Transform>()->position.x == cellPos && invocEntity->get<BoxCollider>()->colliderId == currentPlayerTurn) {
            return true;
        }
    }

    return false;
}

bool GameManager::checkIfEnemyInvocInCell(int cellPosInArray) {
    Entity* cell = cellListRef[cellPosInArray]->getCellEntity();;
    float cellPos = cell->get<Transform>()->position.x;

    for (int i = 0; i < invocationsListRef.size(); i++) {
        Entity* invocEntity = invocationsListRef[i]->getInvocationEntity();
        if (invocEntity->get<Transform>()->position.x == cellPos && invocEntity->get<BoxCollider>()->colliderId != currentPlayerTurn) {
            return true;
        }
    }

    return false;
}

glm::vec3 GameManager::decideRoulColor(int colId) {
    glm::vec3 color1;

    if (colId == 1) {
        color1 = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    else if (colId == 2) {
        color1 = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else {
        color1 = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    return color1;
}

void GameManager::checkBattle(InvocationScript* invocScript) {
    Entity* invocation1 = invocScript->getInvocationEntity();
    ComponentHandle<BoxCollider> collider1 = invocation1->get<BoxCollider>();
    float posXInvoc1 = invocation1->get<Transform>()->position.x;
    float posYInvoc1 = invocation1->get<Transform>()->position.y;

    for (int i = 0; i < invocationsListRef.size(); i++) {
        if (invocationsListRef[i]->getInvocationEntity() == invocation1) {
            posListInvoc1 = i;
            break;
        }
    }

    ComponentHandle<BoxCollider> collider2;
    float posXInvoc2;
    float posYInvoc2;
    bool battleFound = false;

    for (int j = 0; j < invocationsListRef.size(); j++) {
        Entity* invocation2 = invocationsListRef[j]->getInvocationEntity();
        ComponentHandle<BoxCollider> collider2 = invocation2->get<BoxCollider>();
        posXInvoc2 = invocation2->get<Transform>()->position.x;
        posYInvoc2 = invocation2->get<Transform>()->position.y;

        if (posXInvoc1 == posXInvoc2 && collider1->colliderId != collider2->colliderId) {
            posListInvoc2 = j;
            battleFound = true;
            break;
        }
    }

    if (battleFound) {
        battleing = true;
        invocScript->moveInvocation(posXInvoc1 - 40, posYInvoc1);
        invocationsListRef[posListInvoc2]->moveInvocation(posXInvoc2 + 40, posYInvoc2);

        int player1 = invocScript->getPlayerOwner();
        int player2 = invocationsListRef[posListInvoc2]->getPlayerOwner();

        Entity* invoc1 = invocScript->getInvocationEntity();
        ComponentHandle<BoxCollider> col1 = invoc1->get<BoxCollider>();
        int colId1 = col1->colliderId;

        Entity* invoc2 = invocationsListRef[posListInvoc2]->getInvocationEntity();
        ComponentHandle<BoxCollider> col2 = invoc2->get<BoxCollider>();
        int colId2 = col2->colliderId;

        glm::vec3 color1 = decideRoulColor(colId1);
        glm::vec3 color2 = decideRoulColor(colId2);

        float atac1 = invocScript->getAttack();
        float atac2 = invocationsListRef[posListInvoc2]->getAttack();

        spawnRuletaBatalla(color1, color2, atac1, atac2);

        ruletaBatallaSpawned = true;
        invocDestroyed = false;
    }
    checkingIfBattle = false;
}

void GameManager::skipButtonGameLogic() {
    if (!skipButtonIn) {
        for (int j = 0; j < currentPlayerInvocationsListRef.size(); j++) {
            if (currentPlayerInvocationsListRef[j]->getMoveAllowed() && !currentPlayerInvocationsListRef[j]->checkInvocInSpawnCells() && !currentPlayerInvocationsListRef[j]->getMoved()) {
                currentInvocationMoving = j;
                buttonRef->moveSkipButtonIn();
                skipButtonIn = true;
            }
        }
    }

    if (buttonRef->getSkipTurnClicked()) {
        InvocationScript* invocation = currentPlayerInvocationsListRef[currentInvocationMoving];

        handlePassiveEvent<PassiveOne>(invocation, [this, invocation](PassiveOne* passive) {
            passive->OnEvent(this, invocation);
            });

        invocation->setSkippedMovement(true);

        invocation->setMoved(true);
        buttonRef->setSkipTurnClicked(false);
        skipButtonIn = false;
        currentInvocationMoving = -1;
    }

    if (currentInvocationMoving != -1) {
        if (currentPlayerInvocationsListRef[currentInvocationMoving]->getFullyMoved()) {
            buttonRef->moveSkipButtonOut();
            skipButtonIn = false;
            currentInvocationMoving = -1;
        }
    }

    if (currentInvocationMoving != -1) {
        if (currentPlayerInvocationsListRef[currentInvocationMoving]->getRightClickPressed()) {
            buttonRef->moveSkipButtonOut();
            currentPlayerInvocationsListRef[currentInvocationMoving]->setRightClickPressed(false);
            skipButtonIn = false;
            currentInvocationMoving = -1;
        }
    }
}

void GameManager::addPlayerPoints(int playerIndex, int points) {
    if (playerIndex < 1 || playerIndex > 3) {
        std::cerr << "Invalid player index: " << playerIndex << std::endl;
        return;
    }

    playerScores[playerIndex] += points;

    updateScoreVisibility(playerIndex);

    // Debugging output
    std::cout << "Player " << playerIndex << " gained " << points
        << " points. Total: " << playerScores[playerIndex] << " points." << std::endl;
}

void GameManager::awardBattlePoints(int winnerIndex, int loserIndex) {
    int winnerPlayer = invocationsListRef[winnerIndex]->getPlayerOwner();
    int loserPlayer = invocationsListRef[loserIndex]->getPlayerOwner();

    int winnerAttack = invocationsListRef[winnerIndex]->getAttack();
    int loserAttack = invocationsListRef[loserIndex]->getAttack();

    // Calculate attack difference
    int attackDiff = loserAttack - winnerAttack;

    // Award points to the winning player
    playerScores[winnerPlayer] += 2 + std::max(attackDiff, 0);

    // Debugging output
    std::cout << "Battle points awarded! Player " << winnerPlayer
        << " gains " << (2 + std::max(attackDiff, 0)) << " points." << std::endl;
    std::cout << "Updated scores: Player " << winnerPlayer
        << " -> " << playerScores[winnerPlayer] << std::endl;

    updateScoreVisibility(winnerPlayer);
}

void GameManager::awardInvocationPoints(InvocationScript* invocScript) {
    // Get the current player
    int player = invocScript->getPlayerOwner();

    // Get the invocation's attack value
    int attack = invocScript->getAttack();

    // Award points based on the attack
    switch (attack) {
    case 1: 
        playerScores[player] += 3;
        break;
    case 2:
        playerScores[player] += 2;
        break;
    case 3:
        playerScores[player] += 1;
        break;
    case 4:
        // No points for attack 4
        break;
    case 5:
        playerScores[player] -= 1;
        break;
    case 6:
        playerScores[player] -= 2;
        break;
    default:
        std::cout << "Unknown attack value: " << attack << std::endl;
        break;
    }

    // Debugging output
    std::cout << "Invocation points awarded! Player " << player
        << " now has " << playerScores[player] << " points." << std::endl;

    updateScoreVisibility(player);
}

void GameManager::updateScoreVisibility(int playerScoring) {
    if (screenManagerRef && screenManagerRef->getOverlayManager()) {
        screenManagerRef->getOverlayManager()->updateScoreVisibility(playerScoring, playerScores[playerScoring]);
    }
}

void GameManager::addInvocationToBoard(const std::string& invocationId) {
    if (std::find(invocationsOnBoard.begin(), invocationsOnBoard.end(), invocationId) == invocationsOnBoard.end()) {
        invocationsOnBoard.push_back(invocationId);
    }
}

void GameManager::removeInvocationFromBoard(const std::string& invocationId) {
    auto it = std::find(invocationsOnBoard.begin(), invocationsOnBoard.end(), invocationId);
    if (it != invocationsOnBoard.end()) {
        invocationsOnBoard.erase(it);
    }
}

bool GameManager::isInvocationOnBoard(const std::string& invocationId) {
    return std::find(invocationsOnBoard.begin(), invocationsOnBoard.end(), invocationId) != invocationsOnBoard.end();
}

template <typename PassiveType>
void GameManager::handlePassiveEvent(InvocationScript* invocation, std::function<void(PassiveType*)> eventHandler) {
    if (!invocation) return;

    PassiveAbility* passive = invocation->getPassiveAbility();
    if (auto* specificPassive = dynamic_cast<PassiveType*>(passive)) {
        // Call the event handler with the specific passive
        eventHandler(specificPassive);
    }
}

//ENTITY CREATION



void GameManager::spawnCoin() {
    auto result_coin = entityManager->createEntity(glm::vec2(1000.f, 170.f), 0.f, 1.f, "Textures/coin.png", glm::vec3(1., 1., 1.), true, glm::vec2(1.0F, 1.0F), "default", "CoinScript");
    ECS::Entity* coin = result_coin.first;

    Script* generic_script = result_coin.second;

    CoinScript* coin_script = dynamic_cast<CoinScript*>(generic_script);

    coin->assign<BoxCollider>(208.f, 208.f, currentPlayerTurn);
    setCoinRef(coin_script, coin);
    coin_script->setGameManagerRef(this);
}

void GameManager::spawnRuletaAttacks() {
    auto result_ruleta = entityManager->createEntity(glm::vec2(1000.f, 170.f), 0.f, 1.f, "Textures/Ruleta_1.png", glm::vec3(1., 1., 1.), true, glm::vec2(1.0F, 1.0F), "default", "RuletaAttacks");
    ECS::Entity* ruleta = result_ruleta.first;

    Script* generic_script = result_ruleta.second;

    RuletaAttacks* ruleta_script = dynamic_cast<RuletaAttacks*>(generic_script);

    ruleta->assign<BoxCollider>(278.f, 290.f, currentPlayerTurn);
    Entity* pointer_ruleta = entityManager->createEntityWithoutScript(glm::vec2(1000.f, 150.f), 0.f, 1.f, "Textures/puntero_ruleta.png", glm::vec3(1., 1., 1.));
    setRuletaAttacksRef(ruleta_script, ruleta, pointer_ruleta);
}

void GameManager::spawnRuletaBatalla(glm::vec3 color1, glm::vec3 color2, float atac1, float atac2) {
    auto result_ruleta = entityManager->createEntity(glm::vec2(1000.f, 170.f), 0.f, 1.f, "Textures/Ruleta_1.png", glm::vec3(1., 1., 1.), true, glm::vec2(1.0, 1.0), "ruleta", "RuletaBatalla");
    ECS::Entity* ruleta_batalla = result_ruleta.first;

    Script* generic_script = result_ruleta.second;

    RuletaBatalla* ruleta_batalla_script = dynamic_cast<RuletaBatalla*>(generic_script);

    ruleta_batalla->assign<BoxCollider>(278.f, 290.f, currentPlayerTurn);
    ruleta_batalla->assign<RouletteData>(color1, color2, atac1, atac2);
    Entity* pointer_ruleta = entityManager->createEntityWithoutScript(glm::vec2(1000.f, 150.f), 0.f, 1.f, "Textures/puntero_ruleta.png", glm::vec3(1., 1., 1.));
    setRuletaBatallaRef(ruleta_batalla_script, ruleta_batalla, pointer_ruleta);
}

const char* GameManager::returnAttackText(int invocAttack) {
    switch (invocAttack) {
    case 1:
        return "Textures/att1.png";
        break;
    case 2:
        return "Textures/att2.png";
        break;
    case 3:
        return "Textures/att3.png";
        break;
    case 4:
        return "Textures/att4.png";
        break;
    case 5:
        
        return "Textures/att5.png";
        break;
    case 6:
        return "Textures/att6.png";
        break;
    default:
        break;
    }
}



//ENTITY DESTRUCTION



void GameManager::destroyRuletaAttacks() {
    world->destroy(ruletaAttacksEntRef);
    world->destroy(pointerEntRef);
    ruletaAttacksEntRef = nullptr;
    pointerEntRef = nullptr;
    ruletaAttacksRef = nullptr;

    ruletaAttacksSpawned = false;
    ruletaAttacksDestroyed = true;
}

void GameManager::destroyRuletaBatalla() {
    ruletaBatallaWinner = ruletaBatallaRef->getSpinResult();

    std::cout << "got!" << std::endl;

    world->destroy(ruletaBatallaEntRef);
    world->destroy(pointerEntRef);
    ruletaBatallaEntRef = nullptr;
    pointerEntRef = nullptr;
    ruletaBatallaRef = nullptr;

    ruletaBatallaDestroyed = true;
}

void GameManager::eliminateInvocation(int cellPosInArray) {
    InvocationScript* scriptToRemove = invocationsListRef[cellPosInArray];

    PassiveAbility* passive = scriptToRemove->getPassiveAbility();
    if (passive) {
        passive->OnInvocationDeath();
    }


    ECS::Entity* invocationEntity = scriptToRemove->getInvocationEntity();
    if (invocationEntity) {
        ComponentHandle<Sprite> sprite = invocationEntity->get<Sprite>();
        if (sprite) {
            const char* invocationId = sprite->filepath;

            // Remove the invocation from the tracking list
            removeInvocationFromBoard(invocationId);
        }
    }

    invocationsListRef.erase(invocationsListRef.begin() + cellPosInArray);

    scriptToRemove->destroyInvocation();

    //battleing = false;
    //ruletaBatallaDestroyed = false;
}



// LIST FILLING / CLEARING



void GameManager::addInvocationScriptToList(InvocationScript* invsc) {
    invocationsListRef.push_back(invsc);
}

void GameManager::addCellToList(CellScript* cellsc) {
    cellListRef.push_back(cellsc);
}

void GameManager::clearHighlightedList() {
    for (Entity* entity : highlightedEntities) {
        world->destroy(entity);
        //world->destroyEntity(entity);
    }
    highlightedEntities.clear();
}

void GameManager::addInvocationSelectionScriptToList(InvocationSelectionScript* invssc) {
    invocCardListRef.push_back(invssc);
}



//INVOCATION SELECTION



void GameManager::startInvocationSelection() {
    screenManagerRef->activateOverlay(true);
    //activate(false);
    for (auto* script : invocCardListRef) {
        if (isInvocationOnBoard(script->getSpriteFilepath())) {
            script->setUnavailable(true);
        }
        else {
            script->setUnavailable(false);
        }
    }
}

void GameManager::endInvocationSelection() {
    screenManagerRef->activateOverlay(false);
    //activate(true);
}



//GETTERS



int GameManager::getCurrentPlayerTurn() {
    return currentPlayerTurn;
}

std::vector<Entity*> GameManager::getHighlightedEntities() {
    return highlightedEntities;
}

std::vector<Entity*> GameManager::getAllCellEntities() {
    std::vector<Entity*> allCellEntities;
    for (int i = 0; i < cellListRef.size(); i++) {
        allCellEntities.push_back(cellListRef[i]->getCellEntity());
    }
    return allCellEntities;
}

std::vector<CellScript*> GameManager::getAllCellScripts() {
    return cellListRef;
}

std::vector<InvocationScript*>& GameManager::getInvocationScripts() {
    return invocationsListRef; 
}

int GameManager::getInvocationIndex(InvocationScript* invocation) {
    auto it = std::find(invocationsListRef.begin(), invocationsListRef.end(), invocation);
    if (it != invocationsListRef.end()) {
        return std::distance(invocationsListRef.begin(), it);
    }
    return -1; 
}



//SETTERS



/*void GameManager::setRenderSystem(RenderSystem* renderSystem) {
    this->renderSystem = renderSystem;
}*/

void GameManager::setButtonManagerRef(ButtonManager* bmr) {
    buttonRef = bmr;
}

void GameManager::setScreenManagerRef(ScreenManager* smr) {
    screenManagerRef = smr;
}

void GameManager::setRuletaAttacksRef(RuletaAttacks* rcr, Entity* rent, Entity* pointent) {
	ruletaAttacksRef = rcr;
    ruletaAttacksEntRef = rent;
    pointerEntRef = pointent;
}

void GameManager::setRuletaBatallaRef(RuletaBatalla* rbcr, Entity* rbent, Entity* pointent) {
    ruletaBatallaRef = rbcr;
    ruletaBatallaEntRef = rbent;
    pointerEntRef = pointent;
}

void GameManager::setCoinRef(CoinScript* ccr, Entity* cent) {
    coinScriptRef = ccr;
    coinEntRef = cent;
}

void GameManager::setCurrentPlayerInvocationsListRef() {
    for (int i = 0; i < invocationsListRef.size(); i++) {
        Entity* invocationEntity = invocationsListRef[i]->getInvocationEntity();
        ComponentHandle<BoxCollider> collider = invocationEntity->get<BoxCollider>();

        if (collider->colliderId == currentPlayerTurn) {
            InvocationScript* invocationScript = invocationsListRef[i];
            invocationScript->setMovedAndCheckBattleToFalse();

            if (std::find(currentPlayerInvocationsListRef.begin(), currentPlayerInvocationsListRef.end(), invocationScript) == currentPlayerInvocationsListRef.end()) {
                currentPlayerInvocationsListRef.push_back(invocationScript);
            }
        }
    }
}














