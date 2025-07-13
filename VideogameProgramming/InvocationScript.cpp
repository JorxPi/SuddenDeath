#include "InvocationScript.h"
#include "CellConstants.h"
#include "PassiveAbilities.h"

//GENERAL LOGIC



void InvocationScript::startScript() {
	ComponentHandle<Transform> transform = entity->get<Transform>();
	lPos.x = transform->position.x;
	lPos.y = transform->position.y;
	tPos.x = lPos.x;
	tPos.y = lPos.y;
}

void InvocationScript::tickScript(float deltaTime) {

	ComponentHandle<Transform> transform = entity->get<Transform>();
	ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();

	if (recentlySummoned) {
		return;
	}

	if (collider->colliderId == gameRef->getCurrentPlayerTurn()) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			if (mouseX >= transform->position.x - collider->width / 2.0f && mouseX <= transform->position.x + collider->width / 2.0f &&
				mouseY >= transform->position.y - collider->height / 2.0f && mouseY <= transform->position.y + collider->height / 2.0f) {

				if (!moveAllowed && fullyMoved && !gameRef->getAllInvocsMoved() && !hasBattled) {
					buttonRef->moveUndoButtonIn(this); // Show Undo button when already moved
				}

				if (!moved) {
					gameRef->clearHighlightedList();
					highlightCells();
					moveAllowed = true;
				}

			}
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS || (moved && moveAllowed)) {
		clearHighlightedCells();
		rightClickPressed = true;
		buttonRef->moveUndoButtonOut();
	}


	if (moveAllowed) {
		for (int i = 0; i < movableCells.size(); i++) {
			CellScript* cellScript = movableCellScripts[i];
			// Check if cellScript is not nullptr to avoid potential issues
			if (cellScript) {
				moved = cellScript->cellToMove(deltaTime, movableCells, this);
				if (moved) {
					break;
				}
			}
		}
	}

	tPos.x = (tPos.x + lPos.x) / 2;
	tPos.y = (tPos.y + lPos.y) / 2;


	transform->position.x = tPos.x;
	transform->position.y = tPos.y;

	const float positionThreshold = 0.1f;

	if (moved) {
		if (std::abs(transform->position.x - lPos.x) < positionThreshold && std::abs(transform->position.y - lPos.y) < positionThreshold) {
			transform->position.x = lPos.x;
			transform->position.y = lPos.y;
			setFullyMovedToTrue();  
		}
	}
}

void InvocationScript::moveInvocation(int posX, int posY) {
	ComponentHandle<Transform> transform = entity->get<Transform>();
	previousPos = transform->position;
	std::cout << "previousPos " << previousPos.x << " " << previousPos.y << std::endl;

	lPos.x = posX;
	lPos.y = posY;

	canUndo = true;
}

void InvocationScript::highlightCells() {
	ComponentHandle<Transform> transform = entity->get<Transform>();
	glm::vec2 pos = transform->position;

	std::vector<Entity*> allCellEntities = gameRef->getAllCellEntities();
	std::vector<CellScript*> allCellScripts = gameRef->getAllCellScripts();

	auto cellPosInMap = highlightMap.find(pos);
		if (cellPosInMap != highlightMap.end()) {
			for (int index : cellPosInMap->second) {
				if (!gameRef->checkIfAllyInvocInCell(index)) {
					gameRef->highlightSprite(index);
					movableCells.push_back(allCellEntities[index]);
					movableCellScripts.push_back(allCellScripts[index]);
				}
			}
		}

	gameRef->handlePassiveEvent<PassiveFifteen>(this, [this](PassiveFifteen* passiveFifteen) {
		passiveFifteen->ModifyMovableCells(gameRef, this, movableCellScripts, movableCells);
		});
}

void InvocationScript::clearHighlightedCells() {
	gameRef->clearHighlightedList();
	moveAllowed = false;
	movableCells.clear();
	movableCellScripts.clear();
}

/*void InvocationScript::destroyInvocation() {
	entityManager->destroyEntity(cardBackEnt);
	entityManager->destroyEntity(attackNumberEnt);
	entityManager->destroyEntity(invocationEnt);
}*/

bool InvocationScript::checkInvocInSpawnCells() {
	ComponentHandle<Transform> transform = entity->get<Transform>();

	if (lPos == CellPos::ha || lPos == CellPos::hr || lPos == CellPos::hg) {
		return true;
	}
	
	return false;
}

void InvocationScript::updateAttackSprite(const char* newAttackTexture) {
	if (attackNumberEnt) {
		ComponentHandle<Sprite> sprite = attackNumberEnt->get<Sprite>();
		if (sprite) {
			sprite->filepath = newAttackTexture;
		}
	}
}

void InvocationScript::undoLastMove() {
	if (moved) {
		moveAllowed = false;  
		fullyMoved = false;  
		moved = false;     

		moveInvocation(previousPos.x, previousPos.y);

		buttonRef->moveUndoButtonOut();
	}
}

void InvocationScript::incrementSkippedTurns() {
	skippedTurns++;
}

void InvocationScript::resetSkippedTurns() {
	skippedTurns = 0;
}



//GETTERS



bool InvocationScript::getFullyMoved() {
	return fullyMoved;
}

int InvocationScript::getAttack() {
	return attack;
}

bool InvocationScript::getBattleChecked() {
	return battleChecked;
}

Entity* InvocationScript::getInvocationEntity() {
	return invocationEnt;
}

Entity* InvocationScript::getCardBackEntity() {
	return cardBackEnt;
}

Entity* InvocationScript::getAttackNumberEntity() {
	return attackNumberEnt;
}

bool InvocationScript::getMoveAllowed() {
	return moveAllowed;
}

bool InvocationScript::getMoved() {
	return moved;
}

bool InvocationScript::getRightClickPressed() {
	return rightClickPressed;
}

int InvocationScript::getPlayerOwner() {
	return playerOwner;
}

PassiveAbility* InvocationScript::getPassiveAbility() {
	return passiveAbility;
}

int InvocationScript::getSkippedTurns() {
	return skippedTurns;
}

bool InvocationScript::getSkippedMovement() {
	return skippedMovement;
}



//SETTERS



void InvocationScript::setGameManagerRef(GameManager* gmr) {
	gameRef = gmr;
}

void InvocationScript::setButtonManagerRef(ButtonManager* bmr) {
	buttonRef = bmr;
}

void InvocationScript::setInvocRefs(Entity* invr, Entity* cbr, Entity* attr) {
	invocationEnt = invr;
	cardBackEnt = cbr;
	attackNumberEnt = attr;
}

void InvocationScript::setMovedAndCheckBattleToFalse() {
	moved = false;
	battleChecked = false;
	fullyMoved = false;
}
void InvocationScript::setFullyMovedToTrue() {
	fullyMoved = true;
}

void InvocationScript::setMoveAllowedToTrue() {
	moveAllowed = true;
}

void InvocationScript::setInvocAttack(int invAttack) {
	attack = invAttack;

	if (gameRef) {
		const char* attackTexture = gameRef->returnAttackText(attack);
		updateAttackSprite(attackTexture);
	}
}

void InvocationScript::setBattleCheckedToTrue() {
	battleChecked = true;
}

void InvocationScript::setMoved(bool value) {
	moved = value;
}

void InvocationScript::setRightClickPressed(bool value) {
	rightClickPressed = value;
}
void InvocationScript::setPlayerOwner(int owner) {
	playerOwner = owner;
}

void InvocationScript::setPassiveAbility(PassiveAbility* passive) {
	passiveAbility = passive;
	if (passiveAbility) {
		std::cout << "Passive ability set: " << passiveAbility->GetDescription() << std::endl;
	}
	else {
		std::cerr << "Warning: Passive ability set to nullptr." << std::endl;
	}
}

void InvocationScript::setSkippedMovement(bool value) {
	skippedMovement = value;
}

void InvocationScript::setPreviousPos(int posX, int posY) {
	previousPos.x = posX;
	previousPos.y = posY;
}

void InvocationScript::setRecentlySummoned(bool value) { 
	recentlySummoned = value; 
}

void InvocationScript::setHasBattled(bool value) { 
	hasBattled = value; 
}





