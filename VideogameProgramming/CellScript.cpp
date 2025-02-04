#include "CellScript.h"

void CellScript::tickScript(float deltaTime) {

}



//GENERAL LOGIC



bool CellScript::cellToMove(float deltaTime, std::vector<Entity*> cellsToMoveList, InvocationScript* invocationScript) {
	ComponentHandle<Transform> transform = entity->get<Transform>();
	ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (mouseX >= transform->position.x - collider->width / 2.0f && mouseX <= transform->position.x + collider->width / 2.0f &&
			mouseY >= transform->position.y - collider->height / 2.0f && mouseY <= transform->position.y + collider->height / 2.0f) {
			for (Entity* cellEntity : cellsToMoveList) {
				ComponentHandle<Transform> cellTransform = cellEntity->get<Transform>();

				if (cellTransform->position.x == transform->position.x) {
					invocationScript->moveInvocation(transform->position.x, transform->position.y);
					return true;
				}
			}
		}
	}
	return false;
}



//GETTERS



float CellScript::getPosX() {
	ComponentHandle<Transform> transform = entity->get<Transform>();

	return transform->position.x;
}

float CellScript::getPosY() {
	ComponentHandle<Transform> transform = entity->get<Transform>();

	return transform->position.y;
}

Entity* CellScript::getCellEntity() {
	return cellEnt;
}



//SETTERS



void CellScript::setCellRef(Entity* cellr) {
	cellEnt = cellr;
}


