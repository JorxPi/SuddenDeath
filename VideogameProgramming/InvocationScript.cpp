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

	if (collider->colliderId == gameRef->getCurrentPlayerTurn()) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			if (mouseX >= transform->position.x - collider->width / 2.0f && mouseX <= transform->position.x + collider->width / 2.0f &&
				mouseY >= transform->position.y - collider->height / 2.0f && mouseY <= transform->position.y + collider->height / 2.0f) {

				std::cout << "moveAllowed: " << moveAllowed << std::endl;
				std::cout << "fullyMoved: " << fullyMoved << std::endl;
				std::cout << "allInvocsMoved: " << !gameRef->getAllInvocsMoved() << std::endl;

				if (!moveAllowed && fullyMoved && !gameRef->getAllInvocsMoved()) {
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
	//checks if the position was found or not
	if (cellPosInMap != highlightMap.end()) {
		//->second = is the value inside each element in the map, ->first would be the name (ex CellPos::c1))
		for (int index : cellPosInMap->second) {
			if (!gameRef->checkIfAllyInvocInCell(index)) {
				gameRef->highlightSprite(index);
				movableCells.push_back(allCellEntities[index]);
				movableCellScripts.push_back(allCellScripts[index]);
			}
		}
	}
}

void InvocationScript::clearHighlightedCells() {
	gameRef->clearHighlightedList();
	moveAllowed = false;
	movableCells.clear();
	movableCellScripts.clear();
}

void InvocationScript::destroyInvocation() {
	world->destroy(cardBackEnt);
	world->destroy(attackNumberEnt);
	world->destroy(invocationEnt);
}

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

		// Update Transform
		moveInvocation(previousPos.x, previousPos.y);

		// Hide the Undo button after undoing
		buttonRef->moveUndoButtonOut();

		std::cout << "Undoing move, returning to (" << lPos.x << ", " << lPos.y << ")" << std::endl;
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






