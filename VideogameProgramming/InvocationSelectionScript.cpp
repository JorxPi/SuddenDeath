#include "InvocationSelectionScript.h"

void InvocationSelectionScript::tickScript(float deltaTime) {
	if (!overlayManager || overlayManager->isSelectionLocked()) {
		return;  // Ignore clicks if global selection is locked
	}

	ComponentHandle<Transform> transform = entity->get<Transform>();
	ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (mouseX >= transform->position.x - collider->width / 2.0f && mouseX <= transform->position.x + collider->width / 2.0f &&
			mouseY >= transform->position.y - collider->height / 2.0f && mouseY <= transform->position.y + collider->height / 2.0f) {

			std::cout << "Clicked?" << std::endl;

			if (isUnavailable) {
				std::cout << "This invocation is already on the board!" << std::endl;
				return;
			}

			if (discovered) {
				// invoc selected
				card_selected = true;
				setUnavailable(true);

				overlayManager->lockSelection(true);
			}
			else {
				// show the new invoc discovered
				setInvocAsociatedVisible();

				// invoc selected
				card_selected = true;

				setUnavailable(true);

				overlayManager->lockSelection(true);
			}

		}
	}
}

void InvocationSelectionScript::setInvocAsociatedVisible() {
	ComponentHandle<Sprite> sprite_card = entity->get<Sprite>();
	ComponentHandle<Sprite> sprite_invoc = invocAsociated->get<Sprite>();

	sprite_card->filepath = "Textures/neutral_back2.png";

	sprite_invoc->visible = true;

	discovered = true;
}

void InvocationSelectionScript::invocationSelected() {
	ComponentHandle<Sprite> sprite = invocAsociated->get<Sprite>();

	const char* filepath = sprite->filepath;

	// pasar filepath a gameManager
}

void InvocationSelectionScript::asociateInvoc(Entity* enty) {
	invocAsociated = enty;
}

bool InvocationSelectionScript::getCardSelected() const {
	return card_selected;
}

/*const char* InvocationSelectionScript::getSpriteFilepath() const {
	ComponentHandle<Sprite> sprite = invocAsociated->get<Sprite>();

	const char* filepath = sprite->filepath;

	return filepath;
}*/

const char* InvocationSelectionScript::getSpriteFilepath(){
	return filepath.c_str();
}

PassiveAbility* InvocationSelectionScript::getPassiveAbility() {
	return passive;
}

void InvocationSelectionScript::setInvocationData(const std::string& filepath, PassiveAbility* passive) {
	this->filepath = filepath;
	this->passive = passive;
}

void InvocationSelectionScript::setCardSelected(bool status) {
	card_selected = status;
}

void InvocationSelectionScript::setUnavailable(bool unavailable) {
	isUnavailable = unavailable;
	if (unavailableOverlay) {
		auto sprite = unavailableOverlay->get<Sprite>();
		if (sprite) {
			sprite->visible = unavailable;  // Show/hide "InvNotAv" overlay
			std::cout << "Sprite is " << unavailable << std::endl;
		}
	}
}

void InvocationSelectionScript::setUnavailableOverlay(ECS::Entity* overlay) {
	unavailableOverlay = overlay;
}

ECS::Entity* InvocationSelectionScript::getUnavailableOverlay() {
	return unavailableOverlay;
}

void InvocationSelectionScript::setOverlayManager(OverlayManager* manager) {
	overlayManager = manager;
}

