#include "StartGameScreen.h"
#include "CellConstants.h"
#include "InvocationConstants.h"
#include "PassiveAbilities.h"
#include "DummyPassive.h"
#include "InvocationWithPassiveData.h"
#include <vector>
#include <algorithm>
#include <random>

StartGameScreen::StartGameScreen(EntityManager* entityManager, OverlayManager* overlayManager)
	: Screen(entityManager), overlayManager(overlayManager) {
}

void StartGameScreen::enter() {
	entities.push_back(entityManager->createEntityWithoutScript(glm::vec2(600., 400.), 0.f, 1.f, "Textures/GameBack.png", glm::vec3(1., 1., 1.)));

	entities.push_back(entityManager->createCell(CellPos::c1, "Textures/ovalo_normal.png", 74.f, 60.f));

	entities.push_back(entityManager->createCell(CellPos::c2, "Textures/ovalo_normal.png", 74.f, 60.f));

	entities.push_back(entityManager->createCell(CellPos::c3, "Textures/ovalo_normal.png", 74.f, 60.f));

	entities.push_back(entityManager->createCell(CellPos::c4, "Textures/ovalo_normal.png", 74.f, 60.f));
	
	entities.push_back(entityManager->createCell(CellPos::c5, "Textures/ovalo_normal.png", 74.f, 60.f));

	entities.push_back(entityManager->createCell(CellPos::c6, "Textures/ovalo_normal.png", 74.f, 60.f));

	entities.push_back(entityManager->createCell(CellPos::c7, "Textures/ovalo_normal.png", 74.f, 60.f));

	entities.push_back(entityManager->createCell(CellPos::ha, "Textures/hexagono_azul.png", 115.f, 100.f));

	entities.push_back(entityManager->createCell(CellPos::ta, "Textures/triangulo_azul.png", 73.f, 64.f));

	entities.push_back(entityManager->createCell(CellPos::hr, "Textures/hexagono_rojo.png", 115.f, 100.f));

	entities.push_back(entityManager->createCell(CellPos::tr, "Textures/triangulo_rojo.png", 73.f, 64.f));

	entities.push_back(entityManager->createCell(CellPos::hg, "Textures/hexagono_verde.png", 115.f, 100.f));

	entities.push_back(entityManager->createCell(CellPos::tg, "Textures/triangulo_verde.png", 73.f, 64.f));

	//entityManager->createInvocation(glm::vec2(670.f, 110.f), 1, 1, "Textures/blue_back.png", "Textures/inv_Gobobo.png", "Textures/att1.png", 1);

	//entityManager->createInvocation(glm::vec2(950.f, 600.f), 2, 1, "Textures/red_back.png", "Textures/inv_Gobobo.png", "Textures/att1.png", 2);

	//entityManager->createInvocation(glm::vec2(300.f, 570.f), 3, 1, "Textures/green_back.png", "Textures/inv_Gobobo.png", "Textures/att1.png", 3);


	// === Skip Turn Button ===
	auto buttonSkip = entityManager->createEntity(ButtonPos::generalButtonOut, 0.f, 1.f, "Textures/SkipTurnButton.png", glm::vec3(1., 1., 1.), false, glm::vec2(200., 85.), "default", "ButtonScript");
	ECS::Entity* buttonSkipEntity = buttonSkip.first;
	buttonSkipEntity->assign<BoxCollider>(200.f, 85.f, 1);
	entities.push_back(buttonSkipEntity);
	Script* generic_script = buttonSkip.second;

	ButtonScript* buttonSkip_script = dynamic_cast<ButtonScript*>(generic_script);

	if (buttonSkip_script) {
		buttonSkip_script->setOnClickAction([&]() {
			entityManager->getButtonManagerRef()->setSkipTurnClicked(true);
			entityManager->getButtonManagerRef()->moveSkipButtonOut();
		});
	}

	entityManager->getButtonManagerRef()->setSkipButtonRef(buttonSkip_script);

	// === Undo Move Button ===
	auto buttonUndo = entityManager->createEntity(ButtonPos::generalButtonOut, 0.f, 1.f, "Textures/UndoMoveButton.png", glm::vec3(1., 1., 1.), false, glm::vec2(200., 85.), "default", "ButtonScript");
	ECS::Entity* buttonUndoEntity = buttonUndo.first;
	buttonUndoEntity->assign<BoxCollider>(200.f, 85.f, 1);
	entities.push_back(buttonUndoEntity);
	Script* undoGenericScript = buttonUndo.second;

	ButtonScript* buttonUndoScript = dynamic_cast<ButtonScript*>(undoGenericScript);

	if (buttonUndoScript) {
		buttonUndoScript->setOnClickAction([&]() {
			entityManager->getButtonManagerRef()->handleUndoButtonClick();
			});
	}

	entityManager->getButtonManagerRef()->setUndoButtonRef(buttonUndoScript);

	setupOverlayScreen();
	setupScoreScreen();

		
}

void StartGameScreen::exit() {
	for (auto entity : entities) {
		entityManager->destroyEntity(entity);
	}
	entities.clear();
}

std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> StartGameScreen::setupCardBacks() {
	std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> cardBacks;

	glm::vec2 positions[] = {
		glm::vec2(1800.f, 275.f), glm::vec2(1950.f, 275.f), glm::vec2(2100.f, 275.f),
		glm::vec2(2250.f, 275.f), glm::vec2(2400.f, 275.f), glm::vec2(2550.f, 275.f),
		glm::vec2(1800.f, 525.f), glm::vec2(1950.f, 525.f), glm::vec2(2100.f, 525.f),
		glm::vec2(2250.f, 525.f), glm::vec2(2400.f, 525.f), glm::vec2(2550.f, 525.f)
	};

	int numPositions = sizeof(positions) / sizeof(positions[0]);

	for (int i = 0; i < numPositions; i++) {
		auto result = entityManager->createEntity(positions[i], 0.f, 1.f, "Textures/neutral_back1.png", glm::vec3(1., 1., 1.), false, glm::vec2(119., 159.), "default", "InvocationSelectionScript");
		ECS::Entity* cardBack = result.first;
		InvocationSelectionScript* script = dynamic_cast<InvocationSelectionScript*>(result.second);
		cardBack->assign<BoxCollider>(119., 159.);
		cardBack->get<Transform>()->layer = 10;

		ECS::Entity* overlay = entityManager->createEntityWithoutScript(positions[i], 0.f, 1.f, "Textures/InvNotAv.png", glm::vec3(1., 1., 1.), false, glm::vec2(119., 159.));
		overlay->get<Sprite>()->visible = false;
		overlay->get<Transform>()->layer = 11;  

		script->setUnavailableOverlay(overlay);

		cardBacks.push_back(std::make_pair(cardBack, script));
	}

	return cardBacks;
}

std::vector<ECS::Entity*> StartGameScreen::setupInvocations() {
	std::vector<ECS::Entity*> invocations;

	for (int i = 0; i < Invocations::numInvocations; i++) {
		ECS::Entity* invocation = entityManager->createEntityWithoutScript(glm::vec2(0.f, 0.f), 0.f, 1.f, Invocations::data[i].texturePath, glm::vec3(1., 1., 1.), false, Invocations::data[i].size);
		invocation->get<Sprite>()->visible = false;
		invocation->get<Transform>()->layer = 10;
		invocations.push_back(invocation);
	}

	return invocations;
}

void StartGameScreen::randomizeInvocations(std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>>& cardBacks, std::vector<ECS::Entity*>& invocations) {
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(invocations.begin(), invocations.end(), g);

	// Fill with DummyPassive for now
	std::vector<std::unique_ptr<PassiveAbility>> passives;

	// La primera es passiveNine
	passives.emplace_back(std::make_unique<PassiveNine>());

	//segona passiveOne
	passives.emplace_back(std::make_unique<PassiveOne>());

	//tercera passiveFive
	passives.emplace_back(std::make_unique<PassiveFive>());

	//cuarta passiveTen
	passives.emplace_back(std::make_unique<PassiveTen>());

	//cinquena passiveEleven
	passives.emplace_back(std::make_unique<PassiveEleven>());

	//sisena passiveFour
	passives.emplace_back(std::make_unique<PassiveFour>());

	// La resta DummyPassive
	for (size_t i = 6; i < invocations.size(); i++) {
		passives.emplace_back(std::make_unique<DummyPassive>());
	}

	std::shuffle(passives.begin() + 6, passives.end(), g);

	for (size_t i = 0; i < invocations.size() && i < passives.size(); i++) {
		InvocationWithPassiveData data;
		data.filepath = invocations[i]->get<Sprite>()->filepath; // Assign filepath
		data.passive = std::move(passives[i]);                   // Move passive ownership
		invocationPool.push_back(std::move(data));
	}
	

	for (int i = 0; i < cardBacks.size() && i < invocations.size(); i++) {
		InvocationSelectionScript* script = cardBacks[i].second;
		ECS::Entity* invocation = invocations[i];

		invocation->get<Transform>()->position = cardBacks[i].first->get<Transform>()->position;

		script->asociateInvoc(invocations[i]);
		script->setInvocationData(invocationPool[i].filepath, invocationPool[i].passive.get());

		entityManager->settingCardsSelectionList(script);
	}
}

void StartGameScreen::setupOverlayScreen() {
	ECS::Entity* blanket = entityManager->createEntityWithoutScript(glm::vec2(2200.f, 400.f), 0.f, 1.f, "Textures/OverlayBlanket.png", glm::vec3(1., 1., 1.));
	blanket->get<Transform>()->layer = 10;

	std::vector<std::pair<ECS::Entity*, InvocationSelectionScript*>> cardBacks = setupCardBacks();
	std::vector<ECS::Entity*> invocations = setupInvocations();

	randomizeInvocations(cardBacks, invocations);

	overlayManager->addOverlayEntity(blanket);
	for (int i = 0; i < cardBacks.size(); i++) {
		overlayManager->addOverlayEntity(cardBacks[i].first);

		ECS::Entity* overlay = cardBacks[i].second->getUnavailableOverlay(); // Getter for overlay
		if (overlay) {
			overlayManager->addOverlayEntity(overlay); // Add overlay
		}

		cardBacks[i].second->setOverlayManager(overlayManager);
	}
	for (int i = 0; i < invocations.size(); i++) {
		overlayManager->addOverlayEntity(invocations[i]);
	}
}

void StartGameScreen::setupScoreScreen() {
	ECS::Entity* scoreBackground = entityManager->createEntityWithoutScript(glm::vec2(600.f, 400.f),0.f,1.f,"Textures/ScoreTab.png",glm::vec3(1., 1., 1.));
	scoreBackground->get<Sprite>()->visible = false; 
	scoreBackground->get<Transform>()->layer = 20;

	overlayManager->addScoreEntity(-1, scoreBackground);

	glm::vec2 playerColumnStartPositions[] = {
		glm::vec2(459.f, 241.f), 
		glm::vec2(578.f, 241.f), 
		glm::vec2(697.f, 241.f)  
	};

	const int numRows = 20; 
	const int numColumns = 3; 
	const float verticalSpacing = 20.5f; 
	const float horizontalSpacing = 23.f; 

	for (int playerIndex = 0; playerIndex < 3; playerIndex++) {
		glm::vec2 startPos = playerColumnStartPositions[playerIndex];

		for (int row = 0; row < numRows; row++) {
			for (int col = 0; col < numColumns; col++) {
				glm::vec2 position = glm::vec2(
					startPos.x + (col * horizontalSpacing), // X position
					startPos.y + (row * verticalSpacing) // Adjust Y position
				);


				ECS::Entity* playerPoint = entityManager->createEntityWithoutScript(position, 0.f, 1.f, "Textures/SuddenPoint.png", glm::vec3(1., 1., 1.));
				playerPoint->get<Sprite>()->visible = false; // Initially invisible
				playerPoint->get<Transform>()->layer = 21;   // Above the background layer

				// Add it to the overlay manager under the correct player ID
				overlayManager->addScoreEntity(playerIndex + 1, playerPoint); // Player ID: 1, 2, 3
			}
		}
	}
}



/*void StartGameScreen::tickScript(float dt) {

}*/
