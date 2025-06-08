#include "MenuScreen.h"

MenuScreen::MenuScreen(EntityManager* entityManager, OverlayManager* overlayManager)
	: Screen(entityManager, overlayManager) {
}

void MenuScreen::enter() {
	auto e1 = entityManager->createEntityWithoutScript(
		glm::vec2(600.f, 400.f), 0.f, 1.f,
		"Textures/MenuBack.png", glm::vec3(1.f)
	);

	auto e2 = entityManager->createEntityWithoutScript(
		glm::vec2(600.f, 450.f), 0.f, 1.f,
		"Textures/MenuDecor1.png", glm::vec3(1.f), true,
		glm::vec2(1.0f), "menu_motion"
	);

	auto sd1 = e2->get<Sprite>();
	sd1->ampScale = 0.05f;      // ±5%
	sd1->ampAngle = 0.00f;      // no spin
	sd1->freq = 0.5f;       // half a cycle per second
	sd1->phase = 0.0f;       // start at zero

	auto e3 = entityManager->createEntityWithoutScript(
		glm::vec2(600.f, 150.f), 0.f, 1.f,
		"Textures/MenuDecor2.png", glm::vec3(1.f), true,
		glm::vec2(1.0f), "menu_motion"
	);

	auto sd2 = e3->get<Sprite>();
	sd2->ampScale = 0.00f;               // no scale
	sd2->ampAngle = glm::radians(3.0f);  // ±3°
	sd2->freq = 1.2f;
	sd2->phase = glm::radians(90.0f);

	auto e4 = entityManager->createEntityWithoutScript(
		glm::vec2(600.f, 150.f), 0.f, 1.f,
		"Textures/MenuTitle.png", glm::vec3(1.f)
	);

	//entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(900., 400.), 0.f, 1.f, "Textures/MenuDecor3.png", glm::vec3(1., 1., 1.)));

	auto buttonPlay = entityManager->createEntity(
		glm::vec2(900.f, 400.f), 0.f, 1.f,
		"Textures/MenuDecor3.png", glm::vec3(1.f),
		true,                  
		glm::vec2(1.0F, 1.0F),
		"default",             
		"ButtonScript"         
	);	
	ECS::Entity* buttonPlayEntity = buttonPlay.first;

	buttonPlayEntity->assign<BoxCollider>(260.f, 170.f, 1);

	Script* generic_script = buttonPlay.second;
	ButtonScript* buttonPlay_script = dynamic_cast<ButtonScript*>(generic_script);
	if (buttonPlay_script) {
		buttonPlay_script->setOnClickAction([&]() {
			std::cout << "[MenuScreen] Click en Play llama a requestScreenSwap(\"StartGame\")\n";

			requestScreenSwap("StartGame");
			});
	}


	auto ePlayText = entityManager->createEntityWithoutScript(
		glm::vec2(900.f, 400.f), 0.f, 1.f,
		"Textures/MenuPlay.png", glm::vec3(1.f)
	);

	auto eInvGobobo = entityManager->createEntityWithoutScript(
		glm::vec2(720.f, 550.f), 0.f, 1.f,
		"Textures/inv_Gobobo.png", glm::vec3(1.f),
		false, glm::vec2(122.f, 184.f)
	);
}