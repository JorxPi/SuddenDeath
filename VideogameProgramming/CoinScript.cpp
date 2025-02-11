#include "CoinScript.h"

void CoinScript::tickScript(float deltaTime) {
	ComponentHandle<Transform> transform = entity->get<Transform>();
	ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();

	if (!coinFlipped) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			if (mouseX >= transform->position.x - collider->width / 2.0f && mouseX <= transform->position.x + collider->width / 2.0f &&
				mouseY >= transform->position.y - collider->height / 2.0f && mouseY <= transform->position.y + collider->height / 2.0f) {
				if (collider->colliderId == gameRef->getCurrentPlayerTurn()) {

				coinFlipped = true;
				coinFlip();
				}
			}
		}
	}
}



//GENERAL LOGIC



void CoinScript::coinFlip() {
    ComponentHandle<Sprite> sprite = entity->get<Sprite>();

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int summonChance = calculateSummonProbability();

    int randomValue = std::rand() % 100;
    headOrTails = (randomValue < summonChance);  // Compare with probability

    flipTime = std::chrono::steady_clock::now();

    if (headOrTails) {
        std::cout << "Heads!" << std::endl;
        sprite->filepath = "Textures/coin_heads.png";
    }
    else {
        std::cout << "Tails!" << std::endl;
        sprite->filepath = "Textures/coin_tails.png";
    }
}

int CoinScript::calculateSummonProbability() {
    if (!gameRef) return 50; 

    int currentPlayer = gameRef->getCurrentPlayerTurn();

    if (!gameRef->playerHasInvocations(currentPlayer)) {
        return 100;
    }

    int tailsCount = gameRef->getConsecutiveTails(currentPlayer);
    if (tailsCount >= 2) {
        return 100;
    }
    else if (tailsCount == 1) {
        return 75;
    }

    return 50;  
}



//GETTERS



bool CoinScript::getCoinFlipped() {
	return coinFlipped;
}

bool CoinScript::getHeadOrTails() {
	return headOrTails;
}

std::chrono::time_point<std::chrono::steady_clock> CoinScript::getFlipTime() {
	return flipTime;
}



//SETTERS



void CoinScript::setGameManagerRef(GameManager* gmr) {
	gameRef = gmr;
}