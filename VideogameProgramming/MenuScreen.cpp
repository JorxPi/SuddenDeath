#include "MenuScreen.h"

MenuScreen::MenuScreen(EntityManager* entityManager)
	: Screen(entityManager) {}

void MenuScreen::enter() {
	entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(600., 400.), 0.f, 1.f, "Textures/MenuBack.png", glm::vec3(1., 1., 1.)));

	entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(600., 450.), 0.f, 1.f, "Textures/MenuDecor1.png", glm::vec3(1., 1., 1.)));

	entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(600., 150.), 0.f, 1.f, "Textures/MenuDecor2.png", glm::vec3(1., 1., 1.)));

	entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(600., 150.), 0.f, 1.f, "Textures/MenuTitle.png", glm::vec3(1., 1., 1.)));

	//entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(900., 400.), 0.f, 1.f, "Textures/MenuDecor3.png", glm::vec3(1., 1., 1.)));

	auto buttonPlay = entityManager->createEntity(glm::vec2(900., 400.), 0.f, 1.f, "Textures/MenuDecor3.png", glm::vec3(1., 1., 1.), true, glm::vec2(1.0F, 1.0F), "default", "ButtonScript");
	ECS::Entity* buttonPlayEntity = buttonPlay.first;
	buttonPlayEntity->assign<BoxCollider>(260.f, 170.f, 1);
	entities.push_back(buttonPlayEntity);
	Script* generic_script = buttonPlay.second;

	ButtonScript* buttonPlay_script = dynamic_cast<ButtonScript*>(generic_script);

	if (buttonPlay_script) {
		buttonPlay_script->setOnClickAction([&]() {
			std::cout << "Button clicked, this pointer: " << this << std::endl;
			swapToStartGame = true;
		});
	}


	entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(900., 400.), 0.f, 1.f, "Textures/MenuPlay.png", glm::vec3(1., 1., 1.)));

	entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(720., 550.), 0.f, 1.f, "Textures/inv_Gobobo.png", glm::vec3(1., 1., 1.), false, glm::vec2(122., 184.)));

}

void MenuScreen::exit() {
	std::cout << "!!1" << std::endl;
	swapToStartGame = false;
	for (auto entity : entities) {
		entityManager->destroyEntity(entity);
	}
	entities.clear();
}