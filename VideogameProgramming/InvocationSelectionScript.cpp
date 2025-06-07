#include "InvocationSelectionScript.h"
#include "StartGameScreen.h"

void InvocationSelectionScript::tickScript(float deltaTime) {
	// 1) Always do hover logic, regardless of lock
	ComponentHandle<Transform> transform = entity->get<Transform>();
	ComponentHandle<BoxCollider> collider = entity->get<BoxCollider>();

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	bool isHovering = (mouseX >= transform->position.x - collider->width / 2.0f &&
		mouseX <= transform->position.x + collider->width / 2.0f &&
		mouseY >= transform->position.y - collider->height / 2.0f &&
		mouseY <= transform->position.y + collider->height / 2.0f);

	GameManager* gameManager = nullptr;
	if (overlayManager) {
		EntityManager* entityManager = overlayManager->getEntityManager();
		if (entityManager) {
			gameManager = entityManager->getGameManagerRef();
		}
	}

	if (isHovering) {
		if (gameManager->isAnyInvocationHovered() && gameManager->getHoveredInvocation() != this) {
			return;
		}

		if (!isHovered) {
			isHovered = true;
			gameManager->setHoveredInvocation(this);
			updateHoverText();
		}
	}
	else if (isHovered) {
		isHovered = false;
		gameManager->clearHoveredInvocation(this);
	}

	// 2) Only process clicks if not locked
	if (overlayManager && overlayManager->isSelectionLocked(INVOCATIONS_OVERLAY)) {
		return;  // allow hover, but skip below
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && isHovering) {
		if (isUnavailable) {
			std::cout << "This invocation is already on the board!" << std::endl;
			return;
		}
		if (!discovered) {
			setInvocAsociatedVisible();
		}
		card_selected = true;
		setUnavailable(true);

		// Once the player chooses one card, lock further clicks
		overlayManager->lockSelection(INVOCATIONS_OVERLAY, true);

		if (gameManager) {
			gameManager->setHoverText("");
			gameManager->setHoveredInvocation(nullptr);
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

void InvocationSelectionScript::updateHoverText() {
	if (!overlayManager) return;

	EntityManager* entityManager = overlayManager->getEntityManager();
	if (!entityManager) return;

	GameManager* gameManager = entityManager->getGameManagerRef();
	if (!gameManager) return;

	std::string descriptionText;

	if (!discovered) {
		descriptionText = "This invocation has not been revealed yet.";
	}
	else if (passive && !passive->IsRevealed()) {
		descriptionText = "This invocation's passive has not been revealed yet.";
	}
	else if (passive) {
		descriptionText = passive->GetDescription();
	}
	else {
		descriptionText = "This invocation has no passive.";
	}

	gameManager->setHoverText(descriptionText);
}

void InvocationSelectionScript::ResetForNewGame() {
	// 1) Desmarcar cualquier selección pendiente
	card_selected = false;

	// 2) Volver a “no descubierto”
	discovered = false;

	// 3) Marcar como disponible (isUnavailable = false)
	isUnavailable = false;

	// 4) Ocultar el overlay negro si existe
	if (unavailableOverlay) {
		auto sprite = unavailableOverlay->get<Sprite>();
		if (sprite) {
			sprite->visible = false;
		}
	}

	// 5) Si estaba mostrando “hover text”, lo limpiamos
	//    (el GameManager ya borra el hoverText global, pero aseguramos el estado interno)
	isHovered = false;
}

